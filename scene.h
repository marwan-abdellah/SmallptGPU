#ifndef _SCENE_H
#define	_SCENE_H

#include "geomfunc.h"


#define WALL_RAD 1e4f

/**
 * @brief buildCornellBoxScene
 * @return
 */
static Sphere* buildCornellBoxScene()
{
    // Build the cornell box from spheres entirely.
    const int sphereCount = 9;
    int sphereIndex = 0;
    Sphere* cornellScene = (Sphere*) malloc(sizeof(Sphere) * sphereCount);

    // Add the spheres one by one
    Sphere leftWall;
    leftWall.radius = WALL_RAD;
    initVector3(leftWall.position, WALL_RAD + 1.f, 40.8f, 81.6f);
    initVector3(leftWall.emission, 0.f, 0.f, 0.f);
    initVector3(leftWall.color, 1.f, 0.f, 0.f);
    leftWall.reflection = DIFF;
    cornellScene[sphereIndex++] = leftWall;

    Sphere rightWall;
    rightWall.radius = WALL_RAD;
    initVector3(rightWall.position, WALL_RAD + 1.f, 40.8f, 81.6f);
    initVector3(rightWall.emission, 0.f, 0.f, 0.f);
    initVector3(rightWall.color, 0.f, 0.f, 1.0f);
    rightWall.reflection = DIFF;
    cornellScene[sphereIndex++] = rightWall;

    Sphere backWall;
    backWall.radius = WALL_RAD;
    initVector3(backWall.position, WALL_RAD + 1.f, 40.8f, 81.6f);
    initVector3(backWall.emission, 0.f, 0.f, 0.f);
    initVector3(backWall.color, 0.75f, 0.75f, 0.75f);
    backWall.reflection = DIFF;
    cornellScene[sphereIndex++] = backWall;

    Sphere frontWall;
    frontWall.radius = WALL_RAD;
    initVector3(frontWall.position, WALL_RAD + 1.f, 40.8f, 81.6f);
    initVector3(frontWall.emission, 0.f, 0.f, 0.f);
    initVector3(frontWall.color, 0.f, 0.f, 0.f);
    frontWall.reflection = DIFF;
    cornellScene[sphereIndex++] = frontWall;

    Sphere bottomWall;
    bottomWall.radius = WALL_RAD;
    initVector3(bottomWall.position, WALL_RAD + 1.f, 40.8f, 81.6f);
    initVector3(bottomWall.emission, 0.f, 0.f, 0.f);
    initVector3(bottomWall.color, 0.75f, 0.75f, 0.75f);
    bottomWall.reflection = DIFF;
    cornellScene[sphereIndex++] = bottomWall;

    Sphere topWall;
    topWall.radius = WALL_RAD;
    initVector3(topWall.position, WALL_RAD + 1.f, 40.8f, 81.6f);
    initVector3(topWall.emission, 0.f, 0.f, 0.f);
    initVector3(topWall.color, 0.75f, 0.75f, 0.75f);
    topWall.reflection = DIFF;
    cornellScene[sphereIndex++] = topWall;

    Sphere mirrorBall;
    mirrorBall.radius = WALL_RAD;
    initVector3(mirrorBall.position, 50.f, -WALL_RAD + 81.6f, 81.6f)
    initVector3(mirrorBall.emission, 0.f, 0.f, 0.f);
    initVector3(mirrorBall.color, 0.9f, 0.9f, 0.9f);
    mirrorBall.reflection = SPEC;
    cornellScene[sphereIndex++] = mirrorBall;

    Sphere glassBall;
    glassBall.radius = 16.5f;
    initVector3(glassBall.position, 73.f, 16.5f, 78.f);
    initVector3(glassBall.emission, 0.f, 0.f, 0.f);
    initVector3(glassBall.color, 0.9f, 0.9f, 0.9f);
    glassBall.reflection = REFR;
    cornellScene[sphereIndex++] = glassBall;

    Sphere lightSource;
    lightSource.radius = 7.f;
    initVector3(lightSource.position, 50.f, 81.6f - 15.f, 81.6f);
    initVector3(lightSource.emission, 12.f, 12.f, 12.f);
    initVector3(lightSource.color, 0.f, 0.f, 0.f);
    lightSource.reflection = DIFF;
    cornellScene[sphereIndex++] = lightSource;

    return cornellScene;
}



static Sphere CornellSpheres[] = { /* Scene: radius, position, emission, color, material */
    { WALL_RAD, {WALL_RAD + 1.f, 40.8f, 81.6f}, {0.f, 0.f, 0.f}, {.75f, .25f, .25f}, DIFF }, /* Left */
    { WALL_RAD, {-WALL_RAD + 99.f, 40.8f, 81.6f}, {0.f, 0.f, 0.f}, {.25f, .25f, .75f}, DIFF }, /* Rght */
    { WALL_RAD, {50.f, 40.8f, WALL_RAD}, {0.f, 0.f, 0.f}, {.75f, .75f, .75f}, DIFF }, /* Back */
    { WALL_RAD, {50.f, 40.8f, -WALL_RAD + 270.f}, {0.f, 0.f, 0.f}, {0.f, 0.f, 0.f}, DIFF }, /* Frnt */
    { WALL_RAD, {50.f, WALL_RAD, 81.6f}, {0.f, 0.f, 0.f}, {.75f, .75f, .75f}, DIFF }, /* Botm */
    { WALL_RAD, {50.f, -WALL_RAD + 81.6f, 81.6f}, {0.f, 0.f, 0.f}, {.75f, .75f, .75f}, DIFF }, /* Top */
    { 16.5f, {27.f, 16.5f, 47.f}, {0.f, 0.f, 0.f}, {.9f, .9f, .9f}, SPEC }, /* Mirr */
    { 16.5f, {73.f, 16.5f, 78.f}, {0.f, 0.f, 0.f}, {.9f, .9f, .9f}, REFR }, /* Glas */
    { 7.f, {50.f, 81.6f - 15.f, 81.6f}, {12.f, 12.f, 12.f}, {0.f, 0.f, 0.f}, DIFF } /* Lite */
};

#ifdef SCENE_TEST
static const Sphere spheres[] = { /* Scene: radius, position, emission, color, material */
    { 1000.f, {0.f, -1000.f, 0.f}, {0.f, 0.f, 0.f}, {.75f, .75f, .75f}, DIFF }, /* Ground */
    { 15.f, {10.f, 15.f, 0.0f}, {0.f, 0.f, 0.f}, {.75f, 0.f, 0.f}, DIFF }, /* Red */
    { 20.f, {-40.f, 20.f, 0.0f}, {0.f, 0.f, 0.f}, {0.f, 0.f, .75f}, DIFF }, /* Blue */
    { 10.f, {-5.f, 10.f, 20.0f}, {0.f, 0.f, 0.f}, {0.f, .75f, .0f}, DIFF }, /* Blue */
    { 10.f, {-30.f, 100.0f, 20.f}, {12.f, 12.f, 12.f}, {0.f, 0.f, 0.f}, DIFF } /* Lite */
};
#endif

#endif	/* _SCENE_H */

