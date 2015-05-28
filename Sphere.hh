#ifndef SPHERE_HH
#define SPHERE_HH

#include "Vector3.hh"

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

#endif // SPHERE_HH
