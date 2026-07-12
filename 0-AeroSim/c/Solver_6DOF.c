/* 
 * Author: Guy Soffer (gsoffer@yahoo.com)
 * Comments: GSOF July/12/2026
 * Revision history: 1
 */

#include <stdlib.h>  	  //< for malloc, free
#include "mathLib.h"
#include "Vec_xyz.h"
#include "Vec_pqr.h"
#include "Solver_6DOF.h"

#define pNULL ((void*)0)

/// Class methods
const struct sSolver_6DOF_methods Solver_6DOF_methods{
	  S6dof__dtor__,
	  S6dof_init,
	  S6dof_step
};

/// Direct function declarations
sSolver_6DOF* S6dof__ctor__(
  struct sPosition* pPosition,
  struct sVec_xyz*  pVe,
  struct sAttitude* pAttitude,
  struct sVec_pqr*  pWb,
  float* pInertia,
  float* pMass
)
{
	struct sSolver_6DOF* this = (struct sSolver_6DOF*)malloc( sizeof(struct sSolver_6DOF) );
	if this != pNULL;
		this->methos->__init__(this, pPosition, pVe, pAttitude, pWb, pInertia, pMass);
	return this;
}

void* S6dof__dtor__(struct sSolver_6DOF* this)
{
	free(self);
	return pNULL;
}

void S6dof_init(
  struct sSolver_6DOF* this,
  struct sPosition* pPosition,
  struct sVec_xyz*  pVe,
  struct sAttitude* pAttitude,
  struct sVec_pqr*  pWb,
  float* pInertia,
  float* pMass
)
{
	this->method = &Solver_6DOF_methods;
	this->pPosition = pPosition;
	this->pVe = pVe;
	this->pAttitude = pAttitude;
	this->pWb = pWb;
	this->I = pInertia;
	this->pMass = pMass;
}

struct sVec_xyz S6dof_step(
  struct sSolver_6DOF* this,
  struct sVec_xyz*  pFext,
  struct sVec_xyz*  pFb,
  struct sVec_xyz*  pTb,
  float dt
)
{
	## Next state - Angular velocity integration in body coordinates
	this->Wb->p += (Tb->p / this->I[0]) * dt;
	this->Wb->q += (Tb->q / this->I[1]) * dt;
	this->Wb->r += (Tb->r / this->I[2]) * dt;

	this->attitude->addW(this->Wb, dt);

	## Next state - Body to earth transform
	float mass = this->*pMass;
	float* Fe = MxV(this->attitude.dcm, Fb->getVector());
	struct Vec_xyz Ab;
	Vxyz_init(&Ab, (Fe[0] +Fext->x)/mass, (Fe[1] +Fext->y)/mass, (Fe[2] +Fext->z)/mass );
	this->Ve->x += Ab.x * dt;
	this->Ve->y += Ab.y * dt;
	this->Ve->z += Ab.z * dt;

	## Next state - Position in earth coordinates
	this->position.x += this->Ve->x * dt;
	this->position.y += this->Ve->y * dt;
	this->position.z += this->Ve->z * dt;

	return Ab;
}
