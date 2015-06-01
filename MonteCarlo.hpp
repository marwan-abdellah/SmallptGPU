#ifndef MONTECARLO_HPP
#define MONTECARLO_HPP

#include "geom.h"
#include "simplernd.h"
#include "Defines.hh"
#include "Math.hh"
#include "Sphere.hh"
#include "Ray.hh"

/**
 * @brief UniformSampleSphere
 * @param u1 Random number
 * @param u2 Random number
 * @param vec Randomly sampled direction
 */
static void UniformSampleSphere(const float u1, const float u2, Vector3 *vec)
{
    const float zz = 1.f - 2.f * u1;
    const float r = sqrt(max(0.f, 1.f - zz * zz));
    const float phi = 2.f * FLOAT_PI * u2;
    const float xx = r * cos(phi);
    const float yy = r * sin(phi);

    initVector3(*vec, xx, yy, zz);
}

/**
 * @brief UniformSampleDisk
 * @param u1
 * @param u2
 * @param vec
 */
static void UniformSampleDisk(const float u1, const float u2, Vector3 *vec);

#endif // MONTECARLO_HPP
