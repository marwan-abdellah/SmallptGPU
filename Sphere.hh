#ifndef SPHERE_HH
#define SPHERE_HH
#include "Vector3.hh"
#include "Ray.hh"

typedef struct
{
    /** @brief radius */
    float radius;

    /** @brief position */
    Vector3 position;

    /** @brief emission */
    Vector3 emission;

    /** @brief color */
    Vector3 color;

    /** @brief reflection Reflection type (DIFFuse, SPECular, REFRactive) */
    enum Refl reflection;
} Sphere;


/**
 * @brief intersectSphere
 * Sphere ray intersection check.
 * This method solves the parameteric equation of sphere-ray intersection and
 * returns a distance to the sphere on the ray if the ray hits the sphere,
 * otherwise it returns zero.
 * Equation : t^2 * d.d + 2 * t * (o-p).d + (o-p).(o-p) - radius^2 = 0
 * @param sphere
 * @param ray
 * @return
 */
static float intersectSphere(
#ifdef GPU_KERNEL
__constant
#endif
        const Sphere *sphere,
        const Ray *ray)
{
    // Find the ray between the origin of the ray and the sphere center
    // (or position).
    Vector3 originToCenter;
    subtractVectors3(originToCenter, sphere->position, ray->o);

    const float b = dotVectors3(originToCenter, ray->d);
    float det = b * b - dotVectors3(originToCenter, originToCenter) +
            (sphere->radius * sphere->radius);

    if (det < 0.f)
        return 0.f;
    else
        det = sqrt(det);

    float t = b - det;
    if (t > EPSILON)
        return t;
    else
    {
        t = b + det;
        if (t >  EPSILON)
            return t;
        else
            return 0.f;
    }
}
#endif // SPHERE_HH
