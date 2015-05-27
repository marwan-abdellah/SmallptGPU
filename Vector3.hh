#ifndef VECTOR3_HH
#define VECTOR3_HH

// Position, color and normal
typedef struct
{
    float x;
    float y;
    float z;
} Vector3;

typedef Vector3 Color;
typedef Vector3 Point;
typedef Vector3 Normal;

#define initVector3(vec, x0, y0, z0)                                            \
    (vec).x = x0;                                                               \
    (vec).y = y0;                                                               \
    (vec).z = z0;

#define assignVector3(toVec, fromVec)                                           \
    initVector3(toVec, (fromVec).x, (fromVec).y, (fromVec).z)

#define clearVector3(vec)                                                       \
    initVector3(vec, 0.f, 0.f, 0.f)

#define addVectors3(toVec, v1, v2)                                              \
    initVector3(toVec, (v1).x + (v2).x, (v1).y + (v2).y, (v1).z + (v2).z)

#define subtractVectors3(toVec, v1, v2)                                         \
    initVector3(toVec, (v1).x - (v2).x, (v1).y - (v2).y, (v1).z - (v2).z)

#define addVector3Const(toVec, c, fromVec)                                      \
{                                                                               \
    const float val = (c);                                                      \
    initVector3(toVec, (fromVec).x + val, (fromVec).y + val, (fromVec).z + val) \
}

#define subtractVector3Const(toVec, c, fromVec)                                 \
{                                                                               \
    const float val = (c);                                                      \
    initVector3(toVec, (fromVec).x - val, (fromVec).y - val, (fromVec).z - val) \
}

#define multiplyVectors3(toVec, v1, v2)                                         \
    initVector3(toVec, (v1).x * (v2).x, (v1).y * (v2).y, (v1).z * (v2).z);

#define multiplyVector3Const(toVec, c, fromVec)                                 \
{                                                                               \
    const float val = (c);                                                      \
    initVector3(toVec, val * (fromVec).x, val * (fromVec).y, val * (fromVec).z) \
}

#define dotVectors3(v1, v2)                                                     \
    ((v1).x * (v2).x + (v1).y * (v2).y + (v1).z * (v2).z)

#define normVector3(vec)                                                        \
{                                                                               \
    const float len = 1.f / sqrt(dotVectors3(vec, vec));                        \
    multiplyVector3Const(vec, len, vec)                                         \
}

#define crossVectors3(toVec, v1, v2)                                            \
    initVector3(toVec,                                                          \
                (v1).y * (v2).z - (v1).z * (v2).y,                              \
                (v1).z * (v2).x - (v1).x * (v2).z,                              \
                (v1).x * (v2).y - (v1).y * (v2).x)

#define filterVector3(vec)                                                      \
{                                                                               \
    if ((vec).x > (vec).y && (vec).x > (vec).z) return (vec).x;                 \
    else if ((vec).y > (vec).z) return (vec).y;                                 \
    else return (vec).z;                                                        \
}

#define isVector3Zero(vec)                                                            \
    (((vec).x == 0.f) && ((vec).x == 0.f) && ((vec).z == 0.f))

#endif // VECTOR3_HH
