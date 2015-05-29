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

#ifndef _GEOMFUNC_H
#define	_GEOMFUNC_H

#include "geom.h"
#include "simplernd.h"
#include "Defines.hh"
#include "Math.hh"
#include "Sphere.hh"
#include "Ray.hh"

#ifndef SMALLPT_GPU

static float SphereIntersect(
#ifdef GPU_KERNEL
__constant
#endif
    const Sphere *s,
    const Ray *r) { /* returns distance, 0 if nohit */
    Vector3 op; /* Solve t^2*d.d + 2*t*(o-p).d + (o-p).(o-p)-R^2 = 0 */
    subtractVectors3(op, s->position, r->o);

    float b = dotVectors3(op, r->d);
    float det = b * b - dotVectors3(op, op) + s->radius * s->radius;
    if (det < 0.f)
        return 0.f;
    else
        det = sqrt(det);

    float t = b - det;
    if (t >  EPSILON)
        return t;
    else {
        t = b + det;

        if (t >  EPSILON)
            return t;
        else
            return 0.f;
    }
}

static void UniformSampleSphere(const float u1, const float u2, Vector3 *v) {
    const float zz = 1.f - 2.f * u1;
    const float r = sqrt(max(0.f, 1.f - zz * zz));
    const float phi = 2.f * FLOAT_PI * u2;
    const float xx = r * cos(phi);
    const float yy = r * sin(phi);

    initVector3(*v, xx, yy, zz);
}

static int Intersect(
#ifdef GPU_KERNEL
__constant
#endif
    const Sphere *spheres,
    const unsigned int sphereCount,
    const Ray *r,
    float *t,
    unsigned int *id) {
    float inf = (*t) = 1e20f;

    unsigned int i = sphereCount;
    for (; i--;) {
        const float d = SphereIntersect(&spheres[i], r);
        if ((d != 0.f) && (d < *t)) {
            *t = d;
            *id = i;
        }
    }

    return (*t < inf);
}

static int IntersectP(
#ifdef GPU_KERNEL
__constant
#endif
    const Sphere *spheres,
    const unsigned int sphereCount,
    const Ray *r,
    const float maxt) {
    unsigned int i = sphereCount;
    for (; i--;) {
        const float d = SphereIntersect(&spheres[i], r);
        if ((d != 0.f) && (d < maxt))
            return 1;
    }

    return 0;
}

static void SampleLights(
#ifdef GPU_KERNEL
__constant
#endif
    const Sphere *spheres,
    const unsigned int sphereCount,
    unsigned int *seed0, unsigned int *seed1,
    const Vector3 *hitPoint,
    const Vector3 *normal,
    Vector3 *result) {
    clearVector3(*result);

    /* For each light */
    unsigned int i;
    for (i = 0; i < sphereCount; i++) {
#ifdef GPU_KERNEL
__constant
#endif
        const Sphere *light = &spheres[i];
        if (!isVector3Zero(light->emission)) {
            /* It is a light source */
            Ray shadowRay;
            shadowRay.o = *hitPoint;

            /* Choose a point over the light source */
            Vector3 unitSpherePoint;
            UniformSampleSphere(GetRandom(seed0, seed1), GetRandom(seed0, seed1), &unitSpherePoint);
            Vector3 spherePoint;
            multiplyVector3Const(spherePoint, light->radius, unitSpherePoint);
            addVectors3(spherePoint, spherePoint, light->position);

            /* Build the shadow ray direction */
            subtractVectors3(shadowRay.d, spherePoint, *hitPoint);
            const float len = sqrt(dotVectors3(shadowRay.d, shadowRay.d));
            multiplyVector3Const(shadowRay.d, 1.f / len, shadowRay.d);

            float wo = dotVectors3(shadowRay.d, unitSpherePoint);
            if (wo > 0.f) {
                /* It is on the other half of the sphere */
                continue;
            } else
                wo = -wo;

            /* Check if the light is visible */
            const float wi = dotVectors3(shadowRay.d, *normal);
            if ((wi > 0.f) && (!IntersectP(spheres, sphereCount, &shadowRay, len - EPSILON))) {
                Vector3 c; assignVector3(c, light->emission);
                const float s = (4.f * FLOAT_PI * light->radius * light->radius) * wi * wo / (len *len);
                multiplyVector3Const(c, s, c);
                addVectors3(*result, *result, c);
            }
        }
    }
}

static void Radiance(
#ifdef GPU_KERNEL
__constant
#endif
    const Sphere *spheres,
    const unsigned int sphereCount,
    const unsigned int renderingFlags,
    const Ray *startRay,
    unsigned int *seed0, unsigned int *seed1,
    Vector3 *result) {
    Ray currentRay; assignRay(currentRay, *startRay);
    Vector3 rad; initVector3(rad, 0.f, 0.f, 0.f);
    Vector3 throughput; initVector3(throughput, 1.f, 1.f, 1.f);

    unsigned int depth = 0;
    int specularBounce = 1;
    for (;; ++depth) {
        /* Removed Russian Roulette in order to improve execution on SIMD */
        if (depth > 6) {
            *result = rad;
            return;
        }

        float t; /* distance to intersection */
        unsigned int id = 0; /* id of intersected object */
        if (!Intersect(spheres, sphereCount, &currentRay, &t, &id)) {
            *result = rad; /* if miss, return */
            return;
        }

#ifdef GPU_KERNEL
__constant
#endif
        const Sphere *obj = &spheres[id]; /* the hit object */

        Vector3 hitPoint;
        multiplyVector3Const(hitPoint, t, currentRay.d);
        addVectors3(hitPoint, currentRay.o, hitPoint);

        Vector3 normal;
        subtractVectors3(normal, hitPoint, obj->position);
        normVector3(normal);

        float dp = dotVectors3(normal, currentRay.d);
        Vector3 nl;
        if (dp < 0.f) {
            nl = normal;
        } else {
            nl.x = -normal.x;
            nl.y = -normal.y;
            nl.z = -normal.z;
        }

        /* Add emitted light */
        Vector3 eCol; assignVector3(eCol, obj->emission);
        if (!isVector3Zero(eCol)) {
            if (specularBounce) {
                multiplyVector3Const(eCol, fabs(dp), eCol);
                multiplyVectors3(eCol, throughput, eCol);
                addVectors3(rad, rad, eCol);
            }

            *result = rad;
            return;
        }

        if (obj->reflection == DIFF) { /* Ideal DIFFUSE reflection */
            specularBounce = 0;
            multiplyVectors3(throughput, throughput, obj->color);

            /* Direct lighting component */

            Vector3 Ld;
            SampleLights(spheres, sphereCount, seed0, seed1, &hitPoint, &nl, &Ld);
            multiplyVectors3(Ld, throughput, Ld);
            addVectors3(rad, rad, Ld);

            // Check if we have to stop

#ifndef GPU_KERNEL
            // The following lines crash the ATI compiler
            if (renderingFlags & RFLAGS_DISABLE_DIFFUSE_PATH) {
                *result = rad;
                return;
            }
#endif

            /* Diffuse component */

            float r1 = 2.f * FLOAT_PI * GetRandom(seed0, seed1);
            float r2 = GetRandom(seed0, seed1);
            float r2s = sqrt(r2);

            Vector3 w; assignVector3(w, nl);

            Vector3 u;
            if (fabs(w.x) > .1f) {
                Vector3 a;
                initVector3(a, 0.f, 1.f, 0.f);
                crossVectors3(u, a, w);
            } else {
                Vector3 a;
                initVector3(a, 1.f, 0.f, 0.f);
                crossVectors3(u, a, w);
            }
            normVector3(u);

            Vector3 v;
            crossVectors3(v, w, u);

            Vector3 newDir;
            multiplyVector3Const(u, cos(r1) * r2s, u);
            multiplyVector3Const(v, sin(r1) * r2s, v);
            addVectors3(newDir, u, v);
            multiplyVector3Const(w, sqrt(1 - r2), w);
            addVectors3(newDir, newDir, w);

            currentRay.o = hitPoint;
            currentRay.d = newDir;
            continue;
        } else if (obj->reflection == SPEC) { /* Ideal SPECULAR reflection */
            specularBounce = 1;

            Vector3 newDir;
            multiplyVector3Const(newDir,  2.f * dotVectors3(normal, currentRay.d), normal);
            subtractVectors3(newDir, currentRay.d, newDir);

            multiplyVectors3(throughput, throughput, obj->color);

            initRay(currentRay, hitPoint, newDir);
            continue;
        } else {
            specularBounce = 1;

            Vector3 newDir;
            multiplyVector3Const(newDir,  2.f * dotVectors3(normal, currentRay.d), normal);
            subtractVectors3(newDir, currentRay.d, newDir);

            Ray reflRay; initRay(reflRay, hitPoint, newDir); /* Ideal dielectric REFRACTION */
            int into = (dotVectors3(normal, nl) > 0); /* Ray from outside going in? */

            float nc = 1.f;
            float nt = 1.5f;
            float nnt = into ? nc / nt : nt / nc;
            float ddn = dotVectors3(currentRay.d, nl);
            float cos2t = 1.f - nnt * nnt * (1.f - ddn * ddn);

            if (cos2t < 0.f)  { /* Total internal reflection */
                multiplyVectors3(throughput, throughput, obj->color);

                assignRay(currentRay, reflRay);
                continue;
            }

            float kk = (into ? 1 : -1) * (ddn * nnt + sqrt(cos2t));
            Vector3 nkk;
            multiplyVector3Const(nkk, kk, normal);
            Vector3 transDir;
            multiplyVector3Const(transDir, nnt, currentRay.d);
            subtractVectors3(transDir, transDir, nkk);
            normVector3(transDir);

            float a = nt - nc;
            float b = nt + nc;
            float R0 = a * a / (b * b);
            float c = 1 - (into ? -ddn : dotVectors3(transDir, normal));

            float Re = R0 + (1 - R0) * c * c * c * c*c;
            float Tr = 1.f - Re;
            float P = .25f + .5f * Re;
            float RP = Re / P;
            float TP = Tr / (1.f - P);

            if (GetRandom(seed0, seed1) < P) { /* R.R. */
                multiplyVector3Const(throughput, RP, throughput);
                multiplyVectors3(throughput, throughput, obj->color);

                assignRay(currentRay, reflRay);
                continue;
            } else {
                multiplyVector3Const(throughput, TP, throughput);
                multiplyVectors3(throughput, throughput, obj->color);

                initRay(currentRay, hitPoint, transDir);
                continue;
            }
        }
    }
}

#endif

#endif	/* _GEOMFUNC_H */
