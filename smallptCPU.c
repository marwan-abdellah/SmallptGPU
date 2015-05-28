/*
Copyright (c) 2009 David Bucciarelli (davibu@interfree.it)

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be included
in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

/*
 * Based on smallpt, a Path Tracer by Kevin Beason, 2008
 * Modified by David Bucciarelli to show the output via OpenGL/GLUT, ported
 * to C, work with float, fixed RR, ported to OpenCL, etc.
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>

#include "Camera.hh"
#include "scene.h"
#include "GLContext.h"
#include "Externs.hpp"

int workGroupSize = 1;

static Vector3 *colors;
static unsigned int *seeds;
Camera g_camera;
static int currentSample = 0;
Sphere *spheres;
unsigned int sphereCount;

void FreeBuffers() {
    free(seeds);
    free(colors);
    free(g_pixels);
}

void AllocateBuffers() {
    const int pixelCount = g_windowWidth * g_windowHeight;
    int i;
    colors = (Vector3*)malloc(sizeof(Vector3)*pixelCount);

    seeds = (unsigned int*)malloc(sizeof(unsigned int)* pixelCount * 2);
    for (i = 0; i < pixelCount * 2; i++) {
        seeds[i] = rand();
        if (seeds[i] < 2)
            seeds[i] = 2;
    }

    g_pixels = (unsigned int*)malloc(sizeof(unsigned int)* pixelCount);
}

void updateRendering(void) {
    double startTime = profileCurrentStep();

    const float invWidth = 1.f / g_windowWidth;
    const float invHeight = 1.f / g_windowHeight;

    int x, y;
    for (y = 0; y < g_windowHeight; y++) { /* Loop over image rows */
        for (x = 0; x < g_windowWidth; x++) { /* Loop cols */
            const int i = (g_windowHeight - y - 1) * g_windowWidth + x;
            const int i2 = 2 * i;

            const float r1 = GetRandom(&seeds[i2], &seeds[i2 + 1]) - .5f;
            const float r2 = GetRandom(&seeds[i2], &seeds[i2 + 1]) - .5f;
            const float kcx = (x + r1) * invWidth - .5f;
            const float kcy = (y + r2) * invHeight - .5f;

            Vector3 rdir;
            initVector3(rdir,
                g_camera.x.x * kcx + g_camera.y.x * kcy + g_camera.direction.x,
                g_camera.x.y * kcx + g_camera.y.y * kcy + g_camera.direction.y,
                g_camera.x.z * kcx + g_camera.y.z * kcy + g_camera.direction.z);

            Vector3 rorig;
            multiplyVector3Const(rorig, 0.1f, rdir);
            addVectors3(rorig, rorig, g_camera.origin)

            normVector3(rdir);
            const Ray ray = {rorig, rdir};
            Vector3 r;
            Radiance(spheres, sphereCount, renderingFlags, &ray,
                    &seeds[i2], &seeds[i2 + 1], &r);

            if (currentSample == 0)
                colors[i] = r;
            else {
                const float k1 = currentSample;
                const float k2 = 1.f / (k1 + 1.f);
                colors[i].x = (colors[i].x * k1 + r.x) * k2;
                colors[i].y = (colors[i].y * k1 + r.y) * k2;
                colors[i].z = (colors[i].z * k1 + r.z) * k2;
            }

            g_pixels[y * g_windowWidth + x] = toInt(colors[i].x) |
                    (toInt(colors[i].y) << 8) |
                    (toInt(colors[i].z) << 16);
        }
    }

    const float elapsedTime = profileCurrentStep() - startTime;
    const float sampleSec = g_windowHeight * g_windowWidth / elapsedTime;
    sprintf(captionBuffer, "Rendering time %.3f sec (pass %d)  Sample/sec  %.1fK\n",
        elapsedTime, currentSample, sampleSec / 1000.f);

    currentSample++;
}

void reInitScene() {
    currentSample = 0;
}

void reInit(const int reallocBuffers) {
    // Check if I have to reallocate buffers
    if (reallocBuffers) {
        FreeBuffers();
        AllocateBuffers();
    }

    updateCamera();
    currentSample = 0;
    updateRendering();
}

int main(int argc, char *argv[])
{
    // The code will run on the CPU.
    g_device = CPU;

    fprintf(stderr, "Usage: %s\n", argv[0]);
    fprintf(stderr, "Usage: %s <window width> <window height> <scene file>\n", argv[0]);

    if (argc == 4) {
        g_windowWidth = atoi(argv[1]);
        g_windowHeight = atoi(argv[2]);
        readScene(argv[3]);
    } else if (argc == 1) {
        spheres = CornellSpheres;
        sphereCount = sizeof(CornellSpheres) / sizeof(Sphere);

        initVector3(g_camera.origin, 50.f, 45.f, 205.6f);
        initVector3(g_camera.target, 50.f, 45 - 0.042612f, 204.6);
    } else
        exit(-1);

    updateCamera();

    /*------------------------------------------------------------------------*/

    AllocateBuffers();

    /*------------------------------------------------------------------------*/

    initializeGLUT(argc, argv, "SmallPT CPU V1.6alpha (Written by David Bucciarelli)");

    glutMainLoop( );

    return 0;
}
