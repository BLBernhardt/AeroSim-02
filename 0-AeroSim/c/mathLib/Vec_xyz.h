/* 
 * Author: Guy Soffer (gsoffer@yahoo.com)
 * Comments: GSOF July/12/2026
 * Revision history: 1
 */

#ifndef __VEC_XYZ_H
#define __VEC_XYZ_H

struct sVec_xyz {
  const struct sVec_xyz_methods* method; //< Pointer to methods structure
  float x;
  float y;
  float z
};

/// Class methods
struct sVec_xyz_methods {
  void* (*__dtor__)(struct sVec_xyz*),
  void (*init)(struct sVec_xyz*, float, float, float),
  float[3] (*getVector)(struct sVec_xyz*),
  struct sVec_xyz* (*copy)(struct sVec_xyz*),
  float (*mag)(struct sVec_xyz*),
  void (*print)(struct sVec_xyz*)
};

/// Direct function declarations
/// Dynamically allocate memory for the object and initialize object members
sVec_xyz* Vxyz__ctor__(
  float x, //< x component
  float y, //< y component 
  float z, //< z component 
);

/// Release the dynamicaly allocated memory of object
void* Vxyz__dtor__(struct sVec_xyz* this);

/// Initialize the object
void Vxyz_init(
  struct sVec_xyz* this, //< Pointer to object
  float x, //< x component
  float y, //< y component 
  float z, //< z component 
);

float[3] Vpqr_getVector(struct sVec_xyz*);
struct Vxyz_sVec_xyz* copy(struct sVec_xyz*);
float Vxyz_mag(struct sVec_xyz*);
void Vxyz_print(struct sVec_xyz*);

#endif
