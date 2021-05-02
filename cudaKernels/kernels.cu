//
// Created by sivecano on 02/05/2021.
//
#include "cuda_gl_interop.h"
#include "kernels.cuh"
/**
 * calculate which faces within the chunk need to be drawn
 *    _______
 *   /______/|
 *  |      | |
 *  |      | |
 *  |______|/
 *
 *  bitmapping:
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

cudaGraphicsResource_t blockbuffer;

__global__ void calculate_cube_facemask(uint8_t* data)
{
    uint8_t out = 0xff;
    uint x = blockIdx.x * blockDim.x + threadIdx.x;
    uint y = blockIdx.y * blockDim.y + threadIdx.y;
    uint z = blockIdx.z * blockDim.z + threadIdx.z;

    if (x > 0) if (data[(x - 1) + 16 * y + 256*z] == 0) out &= ~8;     // left
    if (y > 0) if (data[x + 16 * (y - 1) + 256*z] == 0) out &= ~1;   // bottom
    if (z > 0) if (data[x + 16 * y + 256*(z - 1)] == 0) out &= ~16;  //front

    if (x < 15) if (data[(x + 1) + 16 * y + 256*z] == 0) out &= ~2;    // right
    if (y < 15) if (data[x + 16 * (y + 1) + 256*z] == 0) out &= ~32; // top
    if (z < 15) if (data[x + 16 * y + 256*(z + 1)] == 0) out &= ~4;  // back

    data[4096 + x + 16 * y + 256 * z] = out;
}


void cudainit() {
    if (cudaSetDevice(0) != cudaSuccess) {
        throw std::runtime_error("cudaSetDevice failed!  Do you have a CUDA-capable GPU installed?");
    }
}

cudaGraphicsResource_t register_buffer(uint buffer)
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

void register_blockbuffer(GLuint buffer) {
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

    dim3 block = {2,2,2};
    dim3 thread = {8,8,8};
    calculate_cube_facemask<<<block, thread>>>((uint8_t*)pointer);
    cudaDeviceSynchronize();
}
