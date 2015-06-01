#include "CLUtilities.h"

/**
 * @brief freeCLBuffer
 * @param clBuffer
 */
void freeCLBuffer(cl_mem clBuffer)
{
    cl_int status = clReleaseMemObject(clBuffer);
    if (status != CL_SUCCESS)
    {
        printf("Failed to release OpenCL color buffer: %d \n", status);
        exit(-1);
    }
}

