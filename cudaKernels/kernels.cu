//
// Created by sivecano on 02/05/2021.
//
#include "cuda.h"
#include "cuda_gl_interop.h"
#include "kernels.cuh"
#include <stdexcept>
#include "stdio.h"

cudaGraphicsResource_t blockbuffer;

__device__ inline unsigned int bindex(unsigned int x, unsigned int y, unsigned int z)
{
    return x + 16 * y + 256 * z;
}

/**
 * calculate which faces within the chunk need to be drawn
 *    _______
 *   /______/|
 *  |      | |
 *  |      | |
 *  |______|/
 *
 *  bitmapping: (from least to most significant bit)
 *
 *  0: bottom
 *  1: right
 *  2: back
 *  3: left
 *  4: front
 *  5: top
 *
 *  we still have to remaining bits that currently serve no use
 *
 *
 * @param buffer data consisting of blocktypes and facemask space
 */
__global__ void calculate_cube_facemask(uint8_t* data)
{
    uint8_t out = 0xff;
    unsigned int x = blockIdx.x * blockDim.x + threadIdx.x;  // location
    unsigned int y = blockIdx.y * blockDim.y + threadIdx.y;
    unsigned int z = blockIdx.z * blockDim.z + threadIdx.z;

    if (x > 0) if (data[bindex(x - 1, y, z)] != 0) out &= ~8;      // left
    if (y > 0) if (data[bindex(x, y - 1, z)] != 0) out &= ~1;      // bottom
    if (z > 0) if (data[bindex(x, y, z - 1)] != 0) out &= ~16;      // front // what happenes if u acess data out of chunk?????? nothing... you just access another part of the array
// oh ic kk
//oh, you mean outside chunk? that's why we have the first "if". we don't access that
    if (x < 15) if (data[bindex(x + 1, y, z)] != 0) out &= ~2;     // right
    if (y < 15) if (data[bindex(x, y + 1, z)] != 0) out &= ~32;    // top
    if (z < 15) if (data[bindex(x, y, z + 1)] != 0) out &= ~4;    // back

    //printf("x: %i, y: %i, z: %i\n", x, y, z);

    if ((x == 16) && (y == 0) && (z == 0)) {
        printf(" curr : %i, x + 1: %i, y + 1: %i, z + 1: %i", data[bindex(x, y, z)], data[bindex(x + 1, y, z)], data[bindex(x, y+1, z)], data[bindex(x, y, z+1)]);
        printf(", x - 1: %i\t", data[bindex(x - 1, y, z)]);
        printf("out: %i\n", out);
    }

    data[4096 + bindex(x, y, z)] = out;
}


void cudainit() {
    if (cudaSetDevice(0) != cudaSuccess) {
        throw std::runtime_error("cudaSetDevice failed!  Do you have a CUDA-capable GPU installed?");
    }
}

cudaGraphicsResource_t register_buffer(int buffer)
{
    cudaGraphicsResource_t resource;
    cudaGraphicsGLRegisterBuffer(&resource, buffer, cudaGraphicsRegisterFlagsNone);
    cudaGraphicsMapResources(1, &resource);
    return resource;
}

void unregister_buffer(cudaGraphicsResource_t resource)
{
    cudaGraphicsUnmapResources(1, &resource);
    cudaGraphicsUnregisterResource(resource);
}

void register_blockbuffer(int buffer) {
    blockbuffer = register_buffer(buffer);
}

void unregister_blockbuffer()
{
    unregister_buffer(blockbuffer);
}

void cube_facemask()
{
    void* pointer;
    size_t size;
    cudaGraphicsResourceGetMappedPointer(&pointer, &size , blockbuffer);
    cudaError_t err;

    dim3 block;
    block.x = 2;
    block.y = 2;
    block.z = 2;

    dim3 thread;
    thread.x = 8;
    thread.y = 8;
    thread.z = 8;

    calculate_cube_facemask<<<block, thread>>>((uint8_t*)pointer);

    err = cudaDeviceSynchronize();
    //printf("return: %s", cudaGetErrorName(err));
    if (err != CUDA_SUCCESS)
        printf("we fucked up majorly: %s\n", cudaGetErrorString(err));
}
