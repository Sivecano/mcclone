//
// Created by sivecano on 04/05/2021.
//

#include "worldgen.cuh"
#include "cuda.h"
#include "stdio.h"

__device__ inline unsigned int bindex(unsigned int x, unsigned int y, unsigned int z)
{
    return x + 16 * z + 256 * y;
}

__global__ void generate(uint8_t* blockids, int chunkx, int chunky, int chunkz)
{
    unsigned int x = blockIdx.x * blockDim.x + threadIdx.x;  // location
    unsigned int y = blockIdx.y * blockDim.y + threadIdx.y;
    unsigned int z = blockIdx.z * blockDim.z + threadIdx.z;

    blockids[bindex(x, y, z)] = ((2*(x + z - 16) < (y + chunky)) || (2*(16 - x + z) < (y + chunky))) ? 0 : 7;
}


void generate_chunkids(uint8_t* chunkids, glm::ivec3 chunkpos)
{
    chunkpos *= 16;

    cudaError_t err;

    dim3 block;
    block.x = 2;
    block.y = 2;
    block.z = 2;

    dim3 thread;
    thread.x = 8;
    thread.y = 8;
    thread.z = 8;

    uint8_t* devptr;

    err = cudaMalloc(&devptr, 4096);

    if (err != CUDA_SUCCESS)
        printf("we fucked up in allocating gpu memory: %s\n", cudaGetErrorString(err));


    generate<<<block, thread>>>(devptr, chunkpos.x, chunkpos.y, chunkpos.z);

    err = cudaDeviceSynchronize();
    //printf("return: %s", cudaGetErrorName(err));
    if (err != CUDA_SUCCESS)
        printf("we fucked up in generating chunk at %i, %i, %i: %s\n", chunkpos.x, chunkpos.y, chunkpos.z, cudaGetErrorString(err));

    err = cudaMemcpy(chunkids, devptr, 4096, cudaMemcpyDeviceToHost);
    cudaFree(devptr);
}