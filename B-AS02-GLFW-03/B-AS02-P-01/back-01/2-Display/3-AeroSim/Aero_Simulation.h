//============================================
//
//		Date: 2024-01-10. Bernhard
//
//===========================================
//
// b_ indicates body coordinates
//
//===========================================

#ifndef FLIGHTMODEL_H
#define FLIGHTMODEL_H
#if 0
#include <stdio.h>
#include <stdio.h>
#include <math.h>
#include <unistd.h> 
#include <termios.h> 
#include <fcntl.h> 
#include <cstring>
#include <math.h>
#endif

#include "../7-MATH/Math.h"

 enum { FPS, METRIC };

 struct CollisionPoint
 {
 	Vector3 ContactPoint;
 	Vector3 Normal;
 	Vector3 NormalW;
 	float Depth;
 };
 	
 	float b_X; // X position in inertial coordinate system (X positive north)
 	float b_Y; // Y position in inertial coordinate system (Y positive east)
 	float b_Z; // Z position in inertial coordinate system (Z positive down)
 
 	float b_Yaw; // yaw angle (positive right)
 	float b_Pitch; // pitch angle (positive up)
 	float b_Roll; // roll angle (positive right)

 	// linear and linear acceleration
 	float b_Vx; 	// X component of velocity in body coordindates (forward)
 	float b_Vy; 	// Y component of velocity in body coordindates (right positive)
 	float b_Vz; 	// Z component of velocity in body coordindates (down positive)
 
 	float b_Ax; 	// acceleration along X axis (forward positive)
 	float b_Ay; 	// acceleration along Y axis (right positive)
 	float b_Az; 	// acceleration along Z axis (down positive)

 	// Force and Toque
 	float b_Fx; 	// force along X axis (forward positive)
 	float b_Fy; 	// force along Y axis (right positive)
 	float b_Fz; 	// force along Z axis (down positive)

 	float b_Tx; 	// torque along X axis (right roll positive)
 	float b_Ty; 	// torque along Y axis (pitch up positive)
 	float b_Tz; 	// torque along Z axis (yaw right positive)
 
 // Angular velocity and acceleration
 	float b_Wx; 	// angular velocity component along X axis (right roll positive)
 	float b_Wy; 	// angular velocity component along Y axis (pitch up positive)
 	float b_Wz; 	// angular velocity component along Z axis (yaw right positive) 
	
 	float b_AAx; 	// angular acceleration along X axis (right roll positive)
 	float b_AAy; 	// angular acceleration along Y axis (pitch up positive)
 	float b_AAz; 	// angular acceleration along Z axis (yaw right positive)
 
 	float b_Mx;	//
 	float b_My;	//
 	float b_Mz;	//

 	double b_Alpha; // angle of attack
 	double b_Beta; // sideslip angle
 	double b_Thrust;
 	float b_Altitude;

 // ------------------------------------

 	double b_rhoSurface;
 	double b_rhoVerticalSurface;
 	double b_airSpeedSq;
 	double b_Weight;

 // Controls - moved to Cntrls data structure
 	double b_Throttle; // throttle [-1, 1].
 	double b_Elevator; // elevator deflection [-1, 1].
 	double b_Ailerons; // aileron deflection [-1, 1].
 	double b_Rudder; // Grudder deflection [-1, 1].

 // ------------------------------------
 // Wind
 	Vector3 Wind; // Gets/Sets wind vector [m/s]

 // ------------------------------------
 // constants
 //const double _AirDensity = 1.2041; // density of air (at sea level and standard pressure). [kg/m3]
 //const double _Gravity = 9.81; // [m/s2]
#endif
//================================================================================================================
// EOF



