//  vector3.h 
#ifndef VECTOR3D_H
#define VECTOR3D_H

#include "Math.h"



/* Construction / Initialization */
void Vector3_Init(Vector3* v);
void Vector3_InitXYZ(Vector3* v, float x, float y, float z);
void Vector3_Copy(Vector3* dest, const Vector3* src);

/* Assignment */
Vector3* Vector3_Assign(Vector3* dest, const Vector3* src);

/* Component Access */
double* Vector3_GetComponent(Vector3* v, unsigned int index);

/* Transformation */
void Vector3_TransformCoordinate(Vector3* v, const struct Matrix* m);

/* Arithmetic Operations */
Vector3 Vector3_Add(const Vector3* a, const Vector3* b);
void    Vector3_AddInPlace(Vector3* a, const Vector3* b);

Vector3 Vector3_MultiplyMatrix(const Vector3* v, const struct Matrix* m);

/* Vector Operations */
Vector3 Vector3_Cross(const Vector3* a, const Vector3* b);
double  Vector3_Dot(const Vector3* a, const Vector3* b);

/* Scalar Operations */
Vector3 Vector3_Scale(float f, const Vector3* v);

#endif /* VECTOR3D_H */
