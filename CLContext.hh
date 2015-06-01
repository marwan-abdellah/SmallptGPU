#ifndef CLCONTEXT_H
#define CLCONTEXT_H

#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/cl.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>

#include "Externs.hpp"
#include "CLUtilities.hh"


/**
 * @brief clFreeBuffers
 */
static void clFreeBuffers(void)
{
    freeCLBuffer(g_colorBufferCL);
    freeCLBuffer(g_pixelBufferCL);
    freeCLBuffer(g_seedBufferCL);

    // TODO: Clean this part
    free(g_seeds);
    free(g_colors);
    free(g_pixels);
}

#endif // CLCONTEXT_H
