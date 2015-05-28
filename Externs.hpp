#ifndef EXTERNS_HPP
#define EXTERNS_HPP

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

// Scene attributes ************************************************************

/** @brief g_camera Main camera in the scene */
extern Camera g_camera;

// Hardware attributes *********************************************************

/** @brief g_device The device which the implementation will run on CPU/GPU */
extern HARDWARE g_device;

#endif // EXTERNS_HPP
