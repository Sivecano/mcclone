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

__global__ void generate(uint8_t* blockids, int64_t chunkx, int64_t chunky, int64_t chunkz)
{
    int64_t x = blockIdx.x * blockDim.x + threadIdx.x;  // location
    int64_t y = blockIdx.y * blockDim.y + threadIdx.y;
    int64_t z = blockIdx.z * blockDim.z + threadIdx.z;

    blockids[bindex(x, y, z)] = (((sin(0.1 * (x + chunkx)) + 1) * abs(x - 8)  + (cos(0.1 * (z + chunkz)) + 1) * abs(z - 8) < (y + chunky) / 2)) ? 0 : 7;
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