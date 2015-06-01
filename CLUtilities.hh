#ifndef CLUTILITIES_H
#define CLUTILITIES_H

#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/cl.h>
#endif

/**
 * @brief allocateCLBufferRW
 * @param currentContext
 * @param sizeBytes
 * @return
 */
static cl_mem allocateCLBufferRW(const cl_context currentContext,
                                 const cl_uint sizeBytes)
{
    cl_int status;
    cl_mem clBuffer = clCreateBuffer(currentContext,
                                     CL_MEM_READ_WRITE,
                                     sizeBytes,
                                     NULL,
                                     &status);
    if (status != CL_SUCCESS)
    {
        printf("Failed to create OpenCL output buffer: %d \n", status);
        exit(-1);
    }

    return clBuffer;
}

/**
 * @brief allocateCLBufferW
 * @param currentContext
 * @param sizeBytes
 * @return
 */
static cl_mem allocateCLBufferW(const cl_context currentContext,
                                const cl_uint sizeBytes)
{
    cl_int status;
    cl_mem clBuffer = clCreateBuffer(currentContext,
                                     CL_MEM_WRITE_ONLY,
                                     sizeBytes,
                                     NULL,
                                     &status);
    if (status != CL_SUCCESS)
    {
        printf("Failed to create OpenCL output buffer: %d \n", status);
        exit(-1);
    }

    return clBuffer;
}

/**
 * @brief freeCLBuffer
 * @param clBuffer
 */
static void freeCLBuffer(cl_mem clBuffer)
{
    cl_int status = clReleaseMemObject(clBuffer);
    if (status != CL_SUCCESS)
    {
        printf("Failed to release OpenCL color buffer: %d \n", status);
        exit(-1);
    }
}

#endif // CLUTILITIES_H
