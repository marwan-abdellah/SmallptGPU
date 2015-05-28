#ifndef APPLICATIONDATA_HPP
#define APPLICATIONDATA_HPP

#include <stdint.h>
#include <stdlib.h>

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

#endif // APPLICATIONDATA_HPP
