//===============================================================================================
// Math.h
//
//===============================================================================================

#ifndef MATH_HEADER_H
#define MATH_HEADER_H


#include <math.h>


#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
     
#define DEG_TO_RAD(angle_in_degrees) ((angle_in_degrees) * M_PI / 180.0)
#define RAD_TO_DEG(angle_in_radians) ((angle_in_radians) * 180.0 / M_PI)

#define DEGtoRAD (M_PI / 180.0)
#define RADtoDEG (180.0 / M_PI)

#define EPSILON 0.000001


#define Sign(x)         (((x) < 0.0) ? (-1.0) : (1.0))


struct _TEST
{
	float  orient_x;
	float  orient_y;
	float  orient_z;
};

typedef struct Vector3 
{
   	double x;
   	double y;
   	double z;
} Vector3;

typedef struct 
{
   	double w; 
   	double x;
   	double y;
   	double z;
} Vector4;

typedef struct Matrix
{
 	int rows;
 	int cols;
 	double *data;
} Matrix;

typedef struct 
{ 
   	double w;
   	double x;
   	double y;
   	double z; 
} Quaternion4;


#endif
//=======================================================================================
// EOF











