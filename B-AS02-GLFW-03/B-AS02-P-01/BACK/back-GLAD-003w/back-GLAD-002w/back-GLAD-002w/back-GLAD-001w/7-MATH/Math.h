//===============================================================================================
// Math.h
//
//    Math.h  is the lowest level or foundation of  mathmatics .h files.   
//
//			Math.h
//			Vector.h
//			Matrix.h
//          Quaternion.h
//
//===============================================================================================

#ifndef MATH_HEADER_H
#define MATH_HEADER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>       


#define DEG_TO_RAD(angle_in_degrees) ((angle_in_degrees) * M_PI / 180.0)
#define RAD_TO_DEG(angle_in_radians) ((angle_in_radians) * 180.0 / M_PI)

#define DEGtoRAD  M_PI/180.0
#define RADtoDEG  180.0/M_PI

#define EPSILON 0.000001



struct _TEST
{
	float  orient_x;
	float  orient_y;
	float  orient_z;
};

/* Vector3 structure */
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

typedef struct 
{
   double p;
   double y;
   double r; 
} Vector3_rate;

#if 0
typedef struct 
{
   double p;
   double y;
   double r; 
} type_vector;
#endif

// 4x4 Matrix structure 
typedef struct Matrix
{
 	int rows;
 	int cols;
 	double *data;
} Matrix;

/* Quaternion structures  */
#if 0
typedef struct 
{
   	double w; 
   	double x;
   	double y;
   	double z;
} Quaternion;
#endif

typedef struct 
{ 
   	double w;
   	double x;
   	double y;
   	double z; 
} Quaternion4;


#endif



