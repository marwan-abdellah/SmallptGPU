#define GPU_KERNEL

#include "Camera.hh"
#include "geomfunc.h"

static void GenerateCameraRay(__constant Camera *g_camera,
        unsigned int *seed0, unsigned int *seed1,
        const int width, const int height, const int x, const int y, Ray *ray) {
    const float invWidth = 1.f / width;
    const float invHeight = 1.f / height;
    const float r1 = GetRandom(seed0, seed1) - .5f;
    const float r2 = GetRandom(seed0, seed1) - .5f;
    const float kcx = (x + r1) * invWidth - .5f;
    const float kcy = (y + r2) * invHeight - .5f;

    Vector3 rdir;
//    setVector3(&rdir, camera->x.x * kcx + camera->y.x * kcy + camera->dir.x,
//                camera->x.y * kcx + camera->y.y * kcy + camera->dir.y,
//               camera->x.z * kcx + camera->y.z * kcy + camera->dir.z);

    initVector3(rdir,
            g_camera->x.x * kcx + g_camera->y.x * kcy + g_camera->direction.x,
            g_camera->x.y * kcx + g_camera->y.y * kcy + g_camera->direction.y,
            g_camera->x.z * kcx + g_camera->y.z * kcy + g_camera->direction.z);

    Vector3 rorig;



    multiplyVector3Const(rorig, 0.1f, rdir);
    // Vector3 rorigx = addVectors3(rorig, camera->orig);
//    rorigx.x = rorig.x;
//    rorigx.y = rorig.y;
//    rorigx.z = rorig.z;

    // addToVector(&rorig, camera->orig);
    addVectors3(rorig, rorig, g_camera->origin);

    // addToVector3(&rorig, camera->orig);


    normVector3(rdir);
    initRay(*ray, rorig, rdir);
}

__kernel void RadianceGPU(
    __global Vector3 *colors, __global unsigned int *seedsInput,
    __constant Sphere *sphere, __constant Camera *g_camera,
    const unsigned int sphereCount,
    const int width, const int height,
    const int currentSample,
    __global int *pixels,
    const unsigned int renderingFlags) {
    const int gid = get_global_id(0);
    const int gid2 = 2 * gid;
    const int x = gid % width;
    const int y = gid / width;

    /* Check if we have to do something */
    if (y >= height)
        return;

    /* LordCRC: move seed to local store */
    unsigned int seed0 = seedsInput[gid2];
    unsigned int seed1 = seedsInput[gid2 + 1];

    Ray ray;
    GenerateCameraRay(g_camera, &seed0, &seed1, width, height, x, y, &ray);

    Vector3 r;
    Radiance(sphere, sphereCount, renderingFlags, &ray, &seed0, &seed1, &r);

    const int i = (height - y - 1) * width + x;
    if (currentSample == 0) {
        // Jens's patch for MacOS
        assignVector3(colors[i], r);
    } else {
        const float k1 = currentSample;
        const float k2 = 1.f / (currentSample + 1.f);
        colors[i].x = (colors[i].x * k1  + r.x) * k2;
        colors[i].y = (colors[i].y * k1  + r.y) * k2;
        colors[i].z = (colors[i].z * k1  + r.z) * k2;
    }

    pixels[y * width + x] = toInt(colors[i].x) |
            (toInt(colors[i].y) << 8) |
            (toInt(colors[i].z) << 16);

    seedsInput[gid2] = seed0;
    seedsInput[gid2 + 1] = seed1;
}
