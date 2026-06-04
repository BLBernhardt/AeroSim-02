//345678901234567890123456789012345678901234567890123456789012345678901234567890
//==============================================================================
//
//       MATH Function,  Vector.cpp
//
//==============================================================================

#include "Vector.h"
#include "Matrix.h"     /* Now safe with forward declaration */




//=====================================================================================================

float vector_dot_product(  Vector3d a,  Vector3d b ) 
{
  return a.x*b.x + a.y*b.y + a.z*b.z;
}

float vector_norm(  Vector3d v ) 
{
  return sqrt( v.x*v.x + v.y*v.y + v.z*v.z );
}

 Vector3d vector_add(  Vector3d a,  Vector3d b  ) 
{
  return (  Vector3d ){ a.x + b.x, a.y + b.y, a.z + b.z };
}

 Vector3d vector_scale( float constant,  Vector3d v ) 
{
  return (  Vector3d ){ constant*v.x, constant*v.y, constant*v.z };
}

 Vector3d vector_normalize(  Vector3d v ) 
{
  return vector_scale( 1/vector_norm( v ), v );
}

 Vector3d vector_cross_product(  Vector3d a,  Vector3d b  ) 
{
  return (  Vector3d ){ a.y*b.z - a.z*b.y, a.x*b.z - a.z*b.x, a.x*b.y - a.y*b.x };
}


//=====================================================================================================


/* Default constructor / initialization */
void Vector3_Init(Vector3* v)
{
    if (v != NULL)
    {
        v->x = 0.0f;
        v->y = 0.0f;
        v->z = 0.0f;
    }
}

/* Constructor with values */
void Vector3_InitXYZ(Vector3* v, float x, float y, float z)
{
    if (v != NULL)
    {
        v->x = x;
        v->y = y;
        v->z = z;
    }
}

/* Copy */
void Vector3_Copy(Vector3* dest, const Vector3* src)
{
    if (dest != NULL && src != NULL)
    {
        *dest = *src;           /* Simple struct copy */
        /* memcpy(dest, src, sizeof(Vector3)); */
    }
}

/* Assignment */
Vector3* Vector3_Assign(Vector3* dest, const Vector3* src)
{
    if (dest != NULL && src != NULL && dest != src)
    {
        *dest = *src;
    }
    return dest;
}

/* Component access (subscript equivalent) */
double* Vector3_GetComponent(Vector3* v, unsigned int index)
{
    if (v == NULL)
        return NULL;

    switch (index)
    {
        case 0:  return &v->x;
        case 1:  return &v->y;
        case 2:  return &v->z;
        default: return NULL;
    }
}

/* In-place transformation using 4x4 matrix (homogeneous coordinates) */
void Vector3_TransformCoordinate(Vector3* v, const struct Matrix* m)
{
    if (v == NULL || m == NULL)
        return;
    
    Vector3 result;
    
    /* Explicit 4x4 transformation */
    result.x = v->x * Matrix_Get(m, 0, 0) +
               v->y * Matrix_Get(m, 1, 0) +
               v->z * Matrix_Get(m, 2, 0) +
               Matrix_Get(m, 3, 0);
    
    result.y = v->x * Matrix_Get(m, 0, 1) +
               v->y * Matrix_Get(m, 1, 1) +
               v->z * Matrix_Get(m, 2, 1) +
               Matrix_Get(m, 3, 1);
    
    result.z = v->x * Matrix_Get(m, 0, 2) +
               v->y * Matrix_Get(m, 1, 2) +
               v->z * Matrix_Get(m, 2, 2) +
               Matrix_Get(m, 3, 2);
    
    /* Write result back to original vector */
    v->x = result.x;
    v->y = result.y;
    v->z = result.z;
}

/* Vector addition */
Vector3 Vector3_Add(const Vector3* a, const Vector3* b)
{
    Vector3 result = {0.0f, 0.0f, 0.0f};
    
    if (a != NULL && b != NULL)
    {
        result.x = a->x + b->x;
        result.y = a->y + b->y;
        result.z = a->z + b->z;
    }
    return result;
}

/* In-place vector addition */
void Vector3_AddInPlace(Vector3* a, const Vector3* b)
{
    if (a != NULL && b != NULL)
    {
        a->x += b->x;
        a->y += b->y;
        a->z += b->z;
    }
}

/* Matrix multiplication (Vector3 * Matrix) */
Vector3 Vector3_MultiplyMatrix(const Vector3* v, const struct Matrix* m)
{
    Vector3 result = {0.0f, 0.0f, 0.0f};
    
    if (v == NULL || m == NULL)
        return result;
    
    /* Explicit 4x4 homogeneous multiplication */
    result.x = v->x * Matrix_Get(m, 0, 0) +
               v->y * Matrix_Get(m, 1, 0) +
               v->z * Matrix_Get(m, 2, 0) +
               Matrix_Get(m, 3, 0);
    
    result.y = v->x * Matrix_Get(m, 0, 1) +
               v->y * Matrix_Get(m, 1, 1) +
               v->z * Matrix_Get(m, 2, 1) +
               Matrix_Get(m, 3, 1);
    
    result.z = v->x * Matrix_Get(m, 0, 2) +
               v->y * Matrix_Get(m, 1, 2) +
               v->z * Matrix_Get(m, 2, 2) +
               Matrix_Get(m, 3, 2);
    
    return result;
}

/* Cross product */
Vector3 Vector3_Cross(const Vector3* a, const Vector3* b)
{
    Vector3 result = {0.0f, 0.0f, 0.0f};
    
    if (a != NULL && b != NULL)
    {
        result.x = a->y * b->z - a->z * b->y;
        result.y = a->z * b->x - a->x * b->z;
        result.z = a->x * b->y - a->y * b->x;
    }
    return result;
}

/* Dot product */
double Vector3_Dot(const Vector3* a, const Vector3* b)
{
    if (a == NULL || b == NULL)
        return 0.0;
    
    return (double)(a->x * b->x + a->y * b->y + a->z * b->z);
}

/* Scalar multiplication */
Vector3 Vector3_Scale(float f, const Vector3* v)
{
    Vector3 result = {0.0f, 0.0f, 0.0f};
    
    if (v != NULL)
    {
        result.x = f * v->x;
        result.y = f * v->y;
        result.z = f * v->z;
    }
    return result;
}

