#include "SceneReader.h"

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

extern void reInit(const int);
extern void reInitScene();
extern void updateRendering();
extern void updateCamera();

extern Sphere *spheres;
extern unsigned int sphereCount;

/**
 * @brief readScene
 * @param fileName
 */
void readScene(char *fileName)
{
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
