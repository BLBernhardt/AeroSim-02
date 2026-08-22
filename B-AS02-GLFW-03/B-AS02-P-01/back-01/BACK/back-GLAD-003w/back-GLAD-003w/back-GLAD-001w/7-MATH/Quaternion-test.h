//=============================================================================
// 7-MATH/Quaternion.h
//   g++ -E -I. -I7-MATH main.c 2>&1 | head -80
//=============================================================================
#ifndef QUATERNION_H
#define QUATERNION_H

#include <math.h>
#include <stdio.h>

#include "Vector.h"
#include "Matrix.h"

// The one and only definition of the type
typedef struct Math_3D
{
    Vector3d        downwards;              // Gravity / down vector
    Vector3d        vehicle_position;
    Quaternion4d    vehicle_orientation_q;  // Current orientation
    Quaternion4d    q_new;                  // radian
    Quaternion4d    q_old;                  // radian
    Quaternion4d    q_dlt;                  // radian (delta)
    Vector3d_rate   angl_new;               // deg
    Vector3d_rate   angl_old;               // deg
    Vector3d_rate   angl_dlt;               // deg
    Quaternion4d    Qtrn_Orient_Disp;       // Orientation display / delta
} Math_3D_t;

extern GLUquadricObj *quadric;

#endif // QUATERNION_H
//=============================================================================
