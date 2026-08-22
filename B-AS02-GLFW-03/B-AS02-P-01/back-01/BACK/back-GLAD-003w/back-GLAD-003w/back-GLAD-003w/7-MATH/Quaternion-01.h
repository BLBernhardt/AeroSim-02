//=========================================================================================
/* quaternion.h */
#ifndef QUATERNION_H
#define QUATERNION_H

#include <math.h>
#include <stdio.h>

#include "Math.h"
#include "Vector.h"
#include "Matrix.h"

typedef struct 
{ 
 	double x, y, z;
} Vec3;

typedef struct 
{ 
 	double w, x, y, z;
} Qtrn4f;

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


 Vector3d quaternion_rotate_vector2( Quaternion4d q,  Vector3d v  );
 //Quaternion4d quaternion_rotate_vector( Quaternion4d q,  Quaternion4d v  ); 
 void Quaternion_Init(Quaternion4d* q);
 void Quaternion_Init(Quaternion4d* q);





 extern GLUquadricObj	*quadric;

 extern Vector3d     downwards;
 extern Vector3d     vehicle_position;

 extern Quaternion4d vehicle_orientation_q;

 extern Quaternion4d q_new; // radian
 extern Quaternion4d q_old; // radian
 extern Quaternion4d q_dlt; // radian
 extern Vector3d_rate  angl_new; // deg   
 extern Vector3d_rate  angl_old; // deg
 extern Vector3d_rate  angl_dlt; // deg  
 extern Quaternion4d Qtrn_Orient_Disp;
 extern float vector_norm( Vector3d );
 extern float vector_dot_product( Vector3d, Vector3d ); 

 Vector3d vector_scale( float, Vector3d  );
 Vector3d vector_add( Vector3d, Vector3d  );
 Vector3d vector_cross_product( Vector3d, Vector3d ); 
 //Vector3d quaternion_rotate_vector( Quaternion4d , Vector3d  );
 Vector3d vector_normalize( Vector3d ); 
 
 Quaternion4d Quat_to_Axis( Quaternion4d );

 Quaternion4d make_rotation_quaternion_from_axis_and_angle( Vector3d, float );
 Quaternion4d quaternion_multiply( Quaternion4d a, Quaternion4d b );
 Quaternion4d quaternion_normalize( Quaternion4d );
 Quaternion4d euler_to_quaternion( double, double, double );
 Quaternion4d euler_to_quaternion( double, double, double );
 Quaternion4d multiply_quaternions( Quaternion4d, Quaternion4d ); 
 void quaternion_to_euler( Quaternion4d, double*, double*, double* );

 void  stroke_output(GLfloat, GLfloat, GLfloat, const char* );

#endif /* QUATERNION_H */
//========================================================



