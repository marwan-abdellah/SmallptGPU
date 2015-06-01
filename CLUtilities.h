#ifndef CLUTILITIES_H
#define CLUTILITIES_H

#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/cl.h>
#endif

/**
 * @brief freeCLBuffer
 * @param clBuffer
 */
void freeCLBuffer(cl_mem clBuffer);

#endif // CLUTILITIES_H
