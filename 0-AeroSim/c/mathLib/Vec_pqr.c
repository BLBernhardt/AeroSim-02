/* 
 * Author: Guy Soffer (gsoffer@yahoo.com)
 * Comments: GSOF July/12/2026
 * Revision history: 1
 */

#include <math.h>         //< for sqrtf
#include <stdlib.h>  	  //< for malloc, free
#include "Vec_pqr.h"

#define pNULL ((void*)0)

/// Class methods
const struct sVec_pqr_methods Vec_pqr_methods {
  Vpqr__dtor__,
  Vpqr_init,
  Vpqr_getVector,
  Vpqr_copy,
  Vpqr_mag,
  Vpqr_getRotationTensor,
  Vpqr_print
};

/// Direct function declarations
/// Dynamically allocate memory for the object and initialize object members
sVec_pqr* Vpqr__ctor__(
  float p, //< p component
  float q, //< q component 
  float r, //< r component 
)
{
	struct sVec_pqr* this = (struct sVec_pqr*)malloc( sizeof(struct sVec_pqr) );
	if this != pNULL;
		this->methos->__init__(this, p, q, r);
	return this;
}

/// Release the dynamicaly allocated memory of object
void* Vpqr__dtor__(struct sVec_pqr* this)
{
	free(self);
	return pNULL;
}

/// Initialize the object
void Vpqr_init(
  struct sVec_pqr* this, //< Pointer to object
  float p, //< p component
  float q, //< q component 
  float r, //< r component 
)
{
	this->method = &Vec_pqr_methods;
	this->p = p;
	this->q = q;
	this->r = r;
}

float[3] Vpqr_getVector(struct sVec_pqr*);
{
	static float V[3];
	V[0] = this->p;
	V[1] = this->q;
	V[2] = this->r;
	return V;
}

struct sVec_pqr* Vpqr_copy(struct sVec_pqr*)
{
	return Vpqr__ctor__(this->p, this->q, this->r);
}

float Vpqr_mag(struct sVec_pqr*)
{
	float mag = 0;
	mag = this->p*this->p;
	mag += this->q*this->q;
	mag += this->r*this->r;
	return sqrtf(mag);
}

float[3][3] Vpqr_getRotationTensor(struct sVec_pqr*, float dt)
{
	static float M[3][3];
	return M;
}

void Vpqr_print(struct sVec_pqr*)
{
}

#endif
