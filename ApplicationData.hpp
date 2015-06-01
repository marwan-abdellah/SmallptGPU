#ifndef APPLICATIONDATA_HPP
#define APPLICATIONDATA_HPP

#include <stdint.h>
#include <stdlib.h>
#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/cl.h>
#endif

#include "Camera.hh"
#include "Defines.hh"
#include "Typedefs.hh"

// Appliaction attributes ******************************************************

/** @brief g_printHelp Prints help message on the OpenGL window */
int g_printHelp;

// GLUT & Image attributes *****************************************************

/** @brief g_windowWidth GLUT window width */
int g_windowWidth;

/** @brief g_windowHeight GLUT window height */
int g_windowHeight;

/** @brief g_pixels A pointer to the image */
uint* g_pixels;

// Scene attributes ************************************************************

/** @brief g_camera Main camera in the scene */
Camera g_camera;

// Hardware attributes *********************************************************

/** @brief g_device The device which the implementation will run on CPU/GPU */
HARDWARE g_device;

// OpenCL attributes ***********************************************************

/** @brief g_clContext An OpenCL active context */
cl_context g_clContext;

/** @brief g_colorBufferCL An OpenCL buffer for the colors */
cl_mem g_colorBufferCL;

/** @brief g_pixelBufferCL An OpenCL buffer for the image */
cl_mem g_pixelBufferCL;

/** @brief g_seedBufferCL An OpenCL buffer for the RNG */
cl_mem g_seedBufferCL;

/** @brief g_sphereBufferCL An OpenCL buffer for the spheres */
cl_mem g_sphereBufferCL;

/** @brief g_cameraBufferCL An OpenCL buffer for the camera */
cl_mem g_cameraBufferCL;


#endif // APPLICATIONDATA_HPP
