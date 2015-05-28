#ifdef WIN32
#define _USE_MATH_DEFINES
#endif
#include <math.h>

#if defined(__linux__) || defined(__APPLE__)
#include <sys/time.h>
#elif defined (WIN32)
#include <windows.h>
#else
Unsupported Platform !!!
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Camera.hh"
#include "Defines.hh"
#include "geom.h"
#include "GLContext.h"
#include "Externs.hpp"
#include "Sphere.hh"
#include "Ray.hh"
#include "GLKeyboardDefines.hh"
#include "GLContext.h"

/**
 * @brief glKeyboard
 * @param key
 * @param x
 * @param y
 */
void glKeyboard(unsigned char key, int x, int y)
{
    switch (key) {
    case 'p': {
        FILE *f = fopen("image.ppm", "w"); // Write image to PPM file.
        if (!f) {
            fprintf(stderr, "Failed to open image file: image.ppm\n");
        } else {
            fprintf(f, "P3\n%d %d\n%d\n", g_windowWidth, g_windowHeight, 255);

            int x, y;
            for (y = g_windowHeight - 1; y >= 0; --y) {
                unsigned char *p = (unsigned char *)(&g_pixels[y * g_windowWidth]);
                for (x = 0; x < g_windowWidth; ++x, p += 4)
                    fprintf(f, "%d %d %d ", p[0], p[1], p[2]);
            }

            fclose(f);
        }
        break;
    }
    case 27: /* Escape key */
        fprintf(stderr, "Done.\n");
        exit(0);
        break;
    case ' ': /* Refresh display */
        reInit(1);
        break;
    case 'a': {
        Vector3 dir = g_camera.x;
        normVector3(dir);
        multiplyVector3Const(dir, -MOVE_STEP, dir);
        addVectors3(g_camera.origin, g_camera.origin, dir);
        addVectors3(g_camera.target, g_camera.target, dir);
        reInit(0);
        break;
    }
    case 'd': {
        Vector3 dir = g_camera.x;
        normVector3(dir);
        multiplyVector3Const(dir, MOVE_STEP, dir);
        addVectors3(g_camera.origin, g_camera.origin, dir);
        addVectors3(g_camera.target, g_camera.target, dir);
        reInit(0);
        break;
    }
    case 'w': {
        Vector3 dir = g_camera.direction;
        multiplyVector3Const(dir, MOVE_STEP, dir);
        addVectors3(g_camera.origin, g_camera.origin, dir);
        addVectors3(g_camera.target, g_camera.target, dir);
        reInit(0);
        break;
    }
    case 's': {
        Vector3 dir = g_camera.direction;
        multiplyVector3Const(dir, -MOVE_STEP, dir);
        addVectors3(g_camera.origin, g_camera.origin, dir);
        addVectors3(g_camera.target, g_camera.target, dir);
        reInit(0);
        break;
    }
    case 'r':
        g_camera.origin.y += MOVE_STEP;
        g_camera.target.y += MOVE_STEP;
        reInit(0);
        break;
    case 'f':
        g_camera.origin.y -= MOVE_STEP;
        g_camera.target.y -= MOVE_STEP;
        reInit(0);
        break;
    case 'l':
        renderingFlags ^= RFLAGS_DISABLE_DIFFUSE_PATH;
        if (renderingFlags & RFLAGS_DISABLE_DIFFUSE_PATH)
            fprintf(stderr, "Indirect diffuse path disable.\n");
        else
            fprintf(stderr, "Indirect diffuse path enable.\n");
        reInit(0);
        break;
    case '+':
        currentSphere = (currentSphere + 1) % sphereCount;
        fprintf(stderr, "Selected sphere %d (%f %f %f)\n", currentSphere,
                spheres[currentSphere].position.x, spheres[currentSphere].position.y, spheres[currentSphere].position.z);
        reInitScene();
        break;
    case '-':
        currentSphere = (currentSphere + (sphereCount - 1)) % sphereCount;
        fprintf(stderr, "Selected sphere %d (%f %f %f)\n", currentSphere,
                spheres[currentSphere].position.x, spheres[currentSphere].position.y, spheres[currentSphere].position.z);
        reInitScene();
        break;
    case '4':
        spheres[currentSphere].position.x -= 0.5f * MOVE_STEP;
        reInitScene();
        break;
    case '6':
        spheres[currentSphere].position.x += 0.5f * MOVE_STEP;
        reInitScene();
        break;
    case '8':
        spheres[currentSphere].position.z -= 0.5f * MOVE_STEP;
        reInitScene();
        break;
    case '2':
        spheres[currentSphere].position.z += 0.5f * MOVE_STEP;
        reInitScene();
        break;
    case '9':
        spheres[currentSphere].position.y += 0.5f * MOVE_STEP;
        reInitScene();
        break;
    case '3':
        spheres[currentSphere].position.y -= 0.5f * MOVE_STEP;
        reInitScene();
        break;
    case 'h':
        g_printHelp = (!g_printHelp);
        break;
    default:
        break;
    }
}
