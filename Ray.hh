#ifndef RAY_HH
#define RAY_HH

#include "Vector3.hh"

typedef struct
{
    /** @brief o */
    Vector3 o;

    /** @brief d */
    Vector3 d;
} Ray;

#define initRay(r, a, b) \
{\
    assignVector3((r).o, a); \
    assignVector3((r).d, b); \
    }

#define assignRay(a, b) \
{\
    assignVector3((a).o, (b).o); \
    assignVector3((a).d, (b).d); \
    }

#endif // RAY_HH
