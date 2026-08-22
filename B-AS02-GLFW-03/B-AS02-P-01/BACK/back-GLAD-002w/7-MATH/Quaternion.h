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
#include <GL/glu.h>

 extern GLUquadricObj	*quadric;
 
 #if 1
 //extern Vector3     	downwards;
 //extern Vector3     	vehicle_position;
 //extern Quaternion4 	vehicle_orientation_q;
 extern Quaternion4 	q_new; // radian
 extern Quaternion4 	q_old; // radian
 extern Quaternion4 	q_dlt; // radian
 extern Vector3_rate  	angl_new; // deg   
 extern Vector3_rate  	angl_old; // deg
 extern Vector3_rate  	angl_dlt; // deg  
 extern Quaternion4 	Qtrn_Orient_Disp;
 #endif
 
 
 typedef struct Math_3D_t
{
    Vector3 downwards;                 // Gravity / down vector
    Vector3 vehicle_position;
    Quaternion4 vehicle_orientation_q; // Current orientation
    Quaternion4 q_new;                 // radian
    Quaternion4 q_old;                 // radian
    Quaternion4 q_dlt;                 // radian (delta)
    Vector3_rate angl_new;             // deg
    Vector3_rate angl_old;             // deg
    Vector3_rate angl_dlt;             // deg
    Quaternion4 Qtrn_Orient_Disp;      // Orientation display / delta
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

float vector_dot_product( Vector3, Vector3 ); 


typedef struct Math_Graphics
{
    Vector3 downwards;                 // Gravity / down vector
    Vector3 vehicle_position;
    Quaternion4 vehicle_orientation_q; // Current orientation
    Quaternion4 q_new;                 // radian
    Quaternion4 q_old;                 // radian
    Quaternion4 q_dlt;                 // radian (delta)
    Vector3_rate angl_new;             // deg
    Vector3_rate angl_old;             // deg
    Vector3_rate angl_dlt;             // deg (delta)
    Quaternion4 Qtrn_Orient_Disp;      // Orientation display / delta
} Math_Graphics_t;








//----------------------------------------------------------------
// Quaternion 

 Quaternion4   quaternion_multiply( Quaternion4 a, Quaternion4 b );
 Quaternion4 	quaternion_conjugate( Quaternion4 q ); 
 Quaternion4 	Quaternion_RotationYawPitchRoll(float yaw, float pitch, float roll);
 
 Vector3 		quaternion_rotate_vector2( Quaternion4 q,  Vector3 v  );
 void           quaternion_to_euler( Quaternion4, double*, double*, double* );
 Quaternion4 	euler_to_quaternion( double, double, double );
 
 
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

 
float vector_norm( Vector3 );
float vector_dot_product( Vector3, Vector3 ); 
 
 Vector3 vector_scale( float, Vector3  );
 Vector3 vector_add( Vector3, Vector3  );
 Vector3 vector_cross_product( Vector3, Vector3 ); 
 Vector3 vector_normalize( Vector3 ); 
#endif


#if 0
/* Initialization */
void 			Quaternion_Init(Quaternion4* q);

/* Operations */
 Quaternion4 	Quaternion_RotationYawPitchRoll(float yaw, float pitch, float roll);
 Quaternion4   make_rotation_quaternion_from_axis_and_angle( Vector3, float );
 Vector3    	Quaternion_ToEulerAngles(const Quaternion4* q);
 Quaternion4	euler_to_quaternion( double, double, double );
 Quaternion4 	euler_to_quaternion( double, double, double );
 void           quaternion_to_euler( Quaternion4, double*, double*, double* );
 
 struct Matrix 	Quaternion_ToTransformMatrix(const Quaternion4* q);
 Quaternion4 	Quaternion_Conjugate(const Quaternion4* q);
 Quaternion4 	quaternion_conjugate( Quaternion4 q ); 
 Quaternion4 	Quaternion_Invert(const Quaternion4* q);
 void       	Quaternion_Normalize(Quaternion4* q);
 Quaternion4   quaternion_normalize( Quaternion4 );
 Quaternion4   quaternion_multiply( Quaternion4 a, Quaternion4 b );
Quaternion4    multiply_quaternions( Quaternion4, Quaternion4 ); 

/* Utility */
void 			Quaternion_Copy(Quaternion4* dest, const Quaternion4* src);
Quaternion4    Quat_to_Axis( Quaternion4 );
Vector3 		quaternion_rotate_vector2( Quaternion4 q,  Vector3 v  );



// void  stroke_output(GLfloat, GLfloat, GLfloat, const char* );
#endif

#endif /* QUATERNION_H */
//========================================================



