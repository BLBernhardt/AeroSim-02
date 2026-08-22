//=========================================================================================
// quaternion.h 
//
//    Math.h  is the lowest level or foundation of  mathmatics .h files.   
//
//			Math.h
//			Vector.h
//			Matrix.h
//          Quaternion.h
//===============================================================================================


#ifndef QUATERNION_H
#define QUATERNION_H

#include <math.h>
#include <stdio.h>
#include "Math.h"
#include "Vector.h"
#include "Matrix.h"


 extern GLUquadricObj	*quadric;
 
 #if 0
 extern Vector3d     	downwards;
 extern Vector3d     	vehicle_position;
 extern Quaternion4d 	vehicle_orientation_q;
 extern Quaternion4d 	q_new; // radian
 extern Quaternion4d 	q_old; // radian
 extern Quaternion4d 	q_dlt; // radian
 extern Vector3d_rate  	angl_new; // deg   
 extern Vector3d_rate  	angl_old; // deg
 extern Vector3d_rate  	angl_dlt; // deg  
 extern Quaternion4d 	Qtrn_Orient_Disp;
 #endif
 
 
typedef struct Math_3D
{
    Vector3d downwards;                 // Gravity / down vector
    Vector3d vehicle_position;
    Quaternion4d vehicle_orientation_q; // Current orientation
    Quaternion4d q_new;                 // radian
    Quaternion4d q_old;                 // radian
    Quaternion4d q_dlt;                 // radian (delta)
    Vector3d_rate angl_new;             // deg
    Vector3d_rate angl_old;             // deg
    Vector3d_rate angl_dlt;             // deg
    Quaternion4d Qtrn_Orient_Disp;      // Orientation display / delta
} Math_3D_t;
 
 


//----------------------------------------------------------------
// Vector 

typedef struct 
{ 
 	double w, x, y, z;
} Qtrn4f;

typedef struct 
{ 
 	double x, y, z;
} Vec3;

float vector_dot_product( Vector3d, Vector3d ); 











//----------------------------------------------------------------
// Quaternion 

 Quaternion4d   quaternion_multiply( Quaternion4d a, Quaternion4d b );
 Quaternion4d 	quaternion_conjugate( Quaternion4d q ); 
 Quaternion4d 	Quaternion_RotationYawPitchRoll(float yaw, float pitch, float roll);
 
 Vector3d 		quaternion_rotate_vector2( Quaternion4d q,  Vector3d v  );
 void           quaternion_to_euler( Quaternion4d, double*, double*, double* );
 Quaternion4d 	euler_to_quaternion( double, double, double );
 
 
 //----------------------------------------------------------------
// Quaternion Trash



#if 0
typedef struct 
{ 
 	double x, y, z;
} Vec3;

typedef struct 
{ 
 	double w, x, y, z;
} Qtrn4f;

 
float vector_norm( Vector3d );
float vector_dot_product( Vector3d, Vector3d ); 
 
 Vector3d vector_scale( float, Vector3d  );
 Vector3d vector_add( Vector3d, Vector3d  );
 Vector3d vector_cross_product( Vector3d, Vector3d ); 
 Vector3d vector_normalize( Vector3d ); 
#endif


#if 0
/* Initialization */
void 			Quaternion_Init(Quaternion4d* q);

/* Operations */
 Quaternion4d 	Quaternion_RotationYawPitchRoll(float yaw, float pitch, float roll);
 Quaternion4d   make_rotation_quaternion_from_axis_and_angle( Vector3d, float );
 Vector3    	Quaternion_ToEulerAngles(const Quaternion4d* q);
 Quaternion4d	euler_to_quaternion( double, double, double );
 Quaternion4d 	euler_to_quaternion( double, double, double );
 void           quaternion_to_euler( Quaternion4d, double*, double*, double* );
 
 struct Matrix 	Quaternion_ToTransformMatrix(const Quaternion4d* q);
 Quaternion4d 	Quaternion_Conjugate(const Quaternion4d* q);
 Quaternion4d 	quaternion_conjugate( Quaternion4d q ); 
 Quaternion4d 	Quaternion_Invert(const Quaternion4d* q);
 void       	Quaternion_Normalize(Quaternion4d* q);
 Quaternion4d   quaternion_normalize( Quaternion4d );
 Quaternion4d   quaternion_multiply( Quaternion4d a, Quaternion4d b );
Quaternion4d    multiply_quaternions( Quaternion4d, Quaternion4d ); 

/* Utility */
void 			Quaternion_Copy(Quaternion4d* dest, const Quaternion4d* src);
Quaternion4d    Quat_to_Axis( Quaternion4d );
Vector3d 		quaternion_rotate_vector2( Quaternion4d q,  Vector3d v  );



// void  stroke_output(GLfloat, GLfloat, GLfloat, const char* );
#endif

#endif /* QUATERNION_H */
//========================================================



