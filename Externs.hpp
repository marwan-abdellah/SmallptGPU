#ifndef EXTERNS_HPP
#define EXTERNS_HPP

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
extern int g_printHelp;

// GLUT & Image attributes *****************************************************

/** @brief g_windowWidth GLUT window width */
extern int g_windowWidth;

/** @brief g_windowHeight GLUT window height */
extern int g_windowHeight;

/** @brief g_pixels A pointer to the image */
extern uint* g_pixels;

/** @brief g_colors The colors of the pixels */
extern Vector3 *g_colors;

/** @brief g_seeds RNG seeds */
extern unsigned int *g_seeds;

// Scene attributes ************************************************************

/** @brief g_camera Main camera in the scene */
extern Camera g_camera;

// Hardware attributes *********************************************************

/** @brief g_device The device which the implementation will run on CPU/GPU */
extern HARDWARE g_device;

// OpenCL attributes ***********************************************************

/** @brief g_clContext An OpenCL active context */
extern cl_context g_clContext;

/** @brief g_colorBufferCL An OpenCL buffer for the colors */
extern cl_mem g_colorBufferCL;

/** @brief g_pixelBufferCL An OpenCL buffer for the image */
extern cl_mem g_pixelBufferCL;

/** @brief g_seedBufferCL An OpenCL buffer for the RNG */
extern cl_mem g_seedBufferCL;

/** @brief g_sphereBufferCL An OpenCL buffer for the spheres */
extern cl_mem g_sphereBufferCL;

/** @brief g_cameraBufferCL An OpenCL buffer for the camera */
extern cl_mem g_cameraBufferCL;

#endif // EXTERNS_HPP
