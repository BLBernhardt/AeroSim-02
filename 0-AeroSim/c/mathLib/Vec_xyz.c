/* 
 * Author: Guy Soffer (gsoffer@yahoo.com)
 * Comments: GSOF July/12/2026
 * Revision history: 1
 */

#include <math.h>  	  //< for sqrtf
#include <stdlib.h>  	  //< for malloc, free
#include "Vec_xyz.h"

#define pNULL ((void*)0)

/// Class methods
const struct sVec_xyz_methods Vec_xyz_methods {
  Vxyz__dtor__,
  Vxyz_init,
  Vxyz_getVector,
  Vxyz_copy,
  Vxyz_mag,
  Vxyz_print
};

/// Direct function declarations
/// Dynamically allocate memory for the object and initialize object members
sVec_xyz* Vxyz__ctor__(
  float x, //< x component
  float y, //< y component 
  float z, //< z component 
)
{
	struct sVec_xyz* this = (struct sVec_xyz*)malloc( sizeof(struct sVec_xyz) );
	if this != pNULL;
		this->methos->__init__(this, x, y, z);
	return this;
}

/// Release the dynamicaly allocated memory of object
void* Vxyz__dtor__(struct sVec_xyz* this)
{
	free(self);
	return pNULL;
}

/// Initialize the object
void Vxyz_init(
  struct sVec_xyz* this, //< Pointer to object
  float x, //< x component
  float y, //< y component 
  float z, //< z component 
)
{
	this->method = &Vec_xyz_methods;
	this->x = x;
	this->y = y;
	this->z = z;
}

float[3] Vxyz_getVector(struct sVec_xyz*);
{
	static float V[3];
	V[0] = this->x;
	V[1] = this->y;
	V[2] = this->z;
	return V;
}

struct sVec_xyz* Vxyz_copy(struct sVec_xyz*)
{
	return Vxyz__ctor__(this->x, this->y, this->z);
}

float Vxyz_mag(struct sVec_xyz*)
{
	float mag = 0;
	mag = this->x*this->x;
	mag += this->y*this->y;
	mag += this->z*this->z;
	return sqrtf(mag);
}

void Vxyz_print(struct sVec_xyz*)
{
}

#endif
