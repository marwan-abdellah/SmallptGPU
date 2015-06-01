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
    FILE *sceneFile;
    fprintf(stderr, "Reading scene: %s \n", fileName);

    // Open the file and parse the data
    sceneFile = fopen(fileName, "r");
    if (!sceneFile)
    {
        fprintf(stderr, "Failed to open file: %s \n", fileName);
        exit(-1);
    }

    // Read the camera attributes
    int attCount = fscanf
            (sceneFile,"camera %f %f %f  %f %f %f\n",
             &g_camera.origin.x, &g_camera.origin.y, &g_camera.origin.z,
             &g_camera.target.x, &g_camera.target.y, &g_camera.target.z);

    if (attCount != 6)
    {
        fprintf(stderr, "Failed to read 6 camera parameters: %d\n", attCount);
        exit(-1);
    }

    // Read the sphere count
    attCount = fscanf(sceneFile,"size %u \n", &sphereCount);
    if (attCount != 1)
    {
        fprintf(stderr, "Failed to read sphere count: %d\n", attCount);
        exit(-1);
    }
    fprintf(stderr, "Scene size: %d\n", sphereCount);

    // Read all the attributes of the spheres
    spheres = (Sphere*) malloc(sizeof(Sphere) * sphereCount);
    unsigned int i;
    for (i = 0; i < sphereCount; i++)
    {
        Sphere *sphereList = &spheres[i];
        int materialIdx;
        attCount = fscanf(sceneFile,"sphere %f %f %f %f %f %f %f %f %f %f %d \n",
                          &sphereList->radius,
                          &sphereList->position.x,
                          &sphereList->position.y,
                          &sphereList->position.z,
                          &sphereList->emission.x,
                          &sphereList->emission.y,
                          &sphereList->emission.z,
                          &sphereList->color.x,
                          &sphereList->color.y,
                          &sphereList->color.z,
                          &materialIdx);

        switch (materialIdx)
        {
        case 0:
            sphereList->reflection = DIFF;
            break;
        case 1:
            sphereList->reflection = SPEC;
            break;
        case 2:
            sphereList->reflection = REFR;
            break;
        default:
            fprintf(stderr, "Failed to read material type for sphere #%d: %d\n",
                    i, materialIdx);
            exit(-1);
            break;
        }
        if (attCount != 11)
        {
            fprintf(stderr, "Failed to read sphere #%d: %d\n", i, attCount);
            exit(-1);
        }
    }

    fclose(sceneFile);
}
