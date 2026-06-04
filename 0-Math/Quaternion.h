//=========================================================================================
/* quaternion.h */
#ifndef QUATERNION_H
#define QUATERNION_H

#include <math.h>
#include <stdio.h>

#include "Math.h"
#include "Vector.h"
#include "Matrix.h"


/* Initialization */
void 			Quaternion_Init(Quaternion4d* q);
void 			Quaternion_InitXYZW(Quaternion4d* q, double x, double y, double z, double w);

/* Operations */
Quaternion4d 	Quaternion_RotationYawPitchRoll(float yaw, float pitch, float roll);


struct Matrix 	Quaternion_ToTransformMatrix(const Quaternion4d* q);


Quaternion4d 	Quaternion_Conjugate(const Quaternion4d* q);
Quaternion4d 	quaternion_conjugate( Quaternion4d q ); 

Quaternion4d 	Quaternion_Invert(const Quaternion4d* q);


void       		Quaternion_Normalize(Quaternion4d* q);


Vector3    		Quaternion_ToEulerAngles(const Quaternion4d* q);

/* Utility */
void 			Quaternion_Copy(Quaternion4d* dest, const Quaternion4d* src);

 Quaternion4d quaternion_rotate_vector( Quaternion4d q,  Quaternion4d v  ); 
 void Quaternion_Init(Quaternion4d* q);
 void Quaternion_Init(Quaternion4d* q);


#endif /* QUATERNION_H */
