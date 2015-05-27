#ifndef CAMERA_HH
#define	CAMERA_HH

#include "Vector3.hh"

typedef struct
{
    /** @brief origin */
    Vector3 origin;

    /** @brief target */
    Vector3 target;

    /** @brief direction */
    Vector3 direction;

    /** @brief x */
    Vector3 x;

    /** @brief y */
    Vector3 y;
} Camera;

#endif	/* CAMERA_HH */

