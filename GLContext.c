#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

#include "Camera.hh"
#include "Defines.hh"
#include "geom.h"
#include "GLContext.h"
#include "Externs.hpp"
#include "Sphere.hh"
#include "Ray.hh"

extern void reInit(const int);
extern void reInitScene();
extern void updateRendering();
extern void updateCamera();

extern Sphere *spheres;
extern unsigned int sphereCount;

unsigned int renderingFlags = 0;
char captionBuffer[256];

static int currentSphere;





/**
 * @brief glPrintHelp
 */
static void glPrintHelp()
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(0.f, 0.f, 0.5f, 0.5f);
    glRecti(40, 40, 600, 440);

    glColor3f(1.f, 1.f, 1.f);
    glRasterPos2i(300, 420);
    glPrintString(HELP_FONT, "Help");

    glRasterPos2i(60, 390);
    glPrintString(HELP_FONT, "h - toggle Help");
    glRasterPos2i(60, 360);
    glPrintString(HELP_FONT, "arrow Keys - rotate camera left/right/up/down");
    glRasterPos2i(60, 330);
    glPrintString(HELP_FONT, "a and d - move camera left and right");
    glRasterPos2i(60, 300);
    glPrintString(HELP_FONT, "w and s - move camera forward and backward");
    glRasterPos2i(60, 270);
    glPrintString(HELP_FONT, "r and f - move camera up and down");
    glRasterPos2i(60, 240);
    glPrintString(HELP_FONT, "PageUp and PageDown - move camera target up and down");
    glRasterPos2i(60, 210);
    glPrintString(HELP_FONT, "+ and - - to select next/previous object");
    glRasterPos2i(60, 180);
    glPrintString(HELP_FONT, "2, 3, 4, 5, 6, 8, 9 - to move selected object");

    glDisable(GL_BLEND);
}

void readScene(char *fileName) {
    FILE *f;
    fprintf(stderr, "Reading scene: %s\n", fileName);

    f = fopen(fileName, "r");
    if (!f) {
        fprintf(stderr, "Failed to open file: %s\n", fileName);
        exit(-1);
    }

    /* Read the camera position */
    int c = fscanf(f,"camera %f %f %f  %f %f %f\n",
                   &g_camera.origin.x, &g_camera.origin.y, &g_camera.origin.z,
                   &g_camera.target.x, &g_camera.target.y, &g_camera.target.z);
    if (c != 6) {
        fprintf(stderr, "Failed to read 6 camera parameters: %d\n", c);
        exit(-1);
    }

    /* Read the sphere count */
    c = fscanf(f,"size %u\n", &sphereCount);
    if (c != 1) {
        fprintf(stderr, "Failed to read sphere count: %d\n", c);
        exit(-1);
    }
    fprintf(stderr, "Scene size: %d\n", sphereCount);

    /* Read all spheres */
    spheres = (Sphere *)malloc(sizeof(Sphere) * sphereCount);
    unsigned int i;
    for (i = 0; i < sphereCount; i++) {
        Sphere *s = &spheres[i];
        int mat;
        int c = fscanf(f,"sphere %f  %f %f %f  %f %f %f  %f %f %f  %d\n",
                       &s->radius,
                       &s->position.x, &s->position.y, &s->position.z,
                       &s->emission.x, &s->emission.y, &s->emission.z,
                       &s->color.x, &s->color.y, &s->color.z,
                       &mat);
        switch (mat) {
        case 0:
            s->reflection = DIFF;
            break;
        case 1:
            s->reflection = SPEC;
            break;
        case 2:
            s->reflection = REFR;
            break;
        default:
            fprintf(stderr, "Failed to read material type for sphere #%d: %d\n", i, mat);
            exit(-1);
            break;
        }
        if (c != 11) {
            fprintf(stderr, "Failed to read sphere #%d: %d\n", i, c);
            exit(-1);
        }
    }

    fclose(f);
}

void updateCamera() {
    subtractVectors3(g_camera.direction, g_camera.target, g_camera.origin);
    normVector3(g_camera.direction);

    const Vector3 up = {0.f, 1.f, 0.f};
    const float fov = (M_PI / 180.f) * 45.f;
    crossVectors3(g_camera.x, g_camera.direction, up);
    normVector3(g_camera.x);
    multiplyVector3Const(g_camera.x, g_windowWidth * fov / g_windowHeight, g_camera.x);

    crossVectors3(g_camera.y, g_camera.x, g_camera.direction);
    normVector3(g_camera.y);
    multiplyVector3Const(g_camera.y, fov, g_camera.y);
}

/**
 * @brief glIdle
 */
void glIdle(void)
{
    // Update the image on a per-frame basis
    updateRendering();

    // Re-run the glDisplay function
    glutPostRedisplay();
}

/**
 * @brief glDisplay
 */
void glDisplay(void)
{
    // Clear the image
    glClear(GL_COLOR_BUFFER_BIT);

    // Start at the orign
    glRasterPos2i(0, 0);

    // Draw the image _pixels_ that contain the rendered frame.
    glDrawPixels(g_windowWidth, g_windowHeight,
                 GL_RGBA, GL_UNSIGNED_BYTE, g_pixels);

    // Title
    glColor3f(1.f, 1.f, 1.f);
    glRasterPos2i(4, g_windowHeight - 16);
    if (g_device == CPU)
        glPrintString(HELP_FONT,
                    "SmallptCPU v1.6alpha (Written by David Bucciarelli)");
    else
        glPrintString(HELP_FONT,
                    "SmallptGPU v1.6alpha (Written by David Bucciarelli)");

    // Caption line 0
    glColor3f(1.f, 1.f, 1.f);
    glRasterPos2i(4, 10);
    glPrintString(HELP_FONT, captionBuffer);

    if (g_printHelp)
    {
        glPushMatrix();
        glLoadIdentity();
        glOrtho(-0.5, 639.5, -0.5, 479.5, -1.0, 1.0);
        glPrintHelp();
        glPopMatrix();
    }

    glutSwapBuffers();
}

/**
 * @brief glReshape
 * @param newWidth
 * @param newHeight
 */
void glReshape(int newWidth, int newHeight)
{
    // Update the global dimensions
    g_windowWidth = newWidth;
    g_windowHeight = newHeight;

    // Update the viewport
    glViewport(0, 0, g_windowWidth, g_windowHeight);
    glLoadIdentity();
    glOrtho(0.f, g_windowWidth - 1.f, 0.f, g_windowHeight - 1.f, -1.f, 1.f);

    // Initialize OpenCL
    reInit(1);

    // Render again
    glutPostRedisplay();
}






/**
 * @brief initializeGLUT
 * @param argc
 * @param argv
 * @param windowTittle
 */
void initializeGLUT(int argc, char *argv[], char *windowTittle)
{
    // Window attributes
    glutInitWindowSize(g_windowWidth, g_windowHeight);
    glutInitWindowPosition(0, 0);

    // Display mode
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInit(&argc, argv);

    // Create the window with specific title
    glutCreateWindow(windowTittle);

    // Register the GLUT functions
    glutReshapeFunc(glReshape);
    glutKeyboardFunc(glKeyboard);
    glutSpecialFunc(glSpecial);
    glutDisplayFunc(glDisplay);
    glutIdleFunc(glIdle);
}
