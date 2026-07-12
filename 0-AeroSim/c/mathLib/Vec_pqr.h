/* 
 * Author: Guy Soffer (gsoffer@yahoo.com)
 * Comments: GSOF July/12/2026
 * Revision history: 1
 */

#ifndef __VEC_PQR_H
#define __VEC_PQR_H

struct sVec_pqr {
  const struct sVec_pqr_methods* method; //< Pointer to methods structure
  float p;
  float q;
  float r
};

/// Class methods
struct sVec_pqr_methods {
  void* (*__dtor__)(struct sVec_pqr*),
  void (*init)(struct sVec_pqr*, float, float, float),
  float[3] (*getVector)(struct sVec_pqr*),
  struct sVec_pqr* (*copy)(struct sVec_pqr*),
  float (*mag)(struct sVec_pqr*),
  float[3][3] (*getRotationTensor)(struct sVec_pqr*, float),
  void (*print)(struct sVec_pqr*)
};

/// Direct function declarations
/// Dynamically allocate memory for the object and initialize object members
sVec_pqr* Vpqr__ctor__(
  float p, //< x component
  float q, //< y component 
  float r, //< z component 
);

/// Release the dynamicaly allocated memory of object
void* Vpqr__dtor__(struct sVec_pqr* this);

/// Initialize the object
void Vpqr_init(
  struct sVec_pqr* this, //< Pointer to object
  float p, //< p component
  float q, //< q component 
  float r, //< r component 
);

float[3] Vpqr_getVector(struct sVec_pqr* this);
struct sVec_xyz* Vpqr_copy(struct sVec_pqr* this);
float Vpqr_mag(struct sVec_pqr* this);
float[3][3] Vpqr_getRotationTensor(struct sVec_pqr* this, float dt);
void Vpqr_print(struct sVec_pqr* this);

#endif
