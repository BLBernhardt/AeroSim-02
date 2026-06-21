//============================================
//
//		Original Code:  Ed LeBouthhiller
//		Date: 2024-01-01
//
// 		Updates
//		Date:  2024-01-10.   Bernhard
//		Description: Added comments - Body coordinate system
//
//
//
//
//===========================================
//
//     b_   indicates body coordinates
//
//===========================================

#ifndef FLIGHTMODEL_H
#define FLIGHTMODEL_H

#include <stdio.h>
#include <stdio.h>
#include <math.h>
#include <unistd.h> 
#include <termios.h> 
#include <fcntl.h> 
#include <cstring>
#include <math.h>


#include "../0-Math/Math.h"
#include "../0-Math/Vector.h"
#include "../0-Math/Matrix.h"
#include "../0-Math/Quaternion.h"

//namespace FCSim
//{
//   class FlightModel
//   {
//   public:
//      FlightModel();
      

      
 //     	void  set_X(float x) { b_X = x; };
 //     	float get_X() { return b_X; };

      //	void Initialize();
      //	void MoveScene(float elapsedTime);
      	


 //     	bool Load(string filename);
      
 //   private:
      
		struct CollisionPoint
        {
            Vector3 ContactPoint;
            Vector3 Normal;
            Vector3 NormalW;
            float Depth;
        };

  		//Quaternion b_OrientationQuat;

      	//Vector3    b_Velocity;
      	
      	
      	//float b_FP_ang;// flight path angle
      	
      	float b_X;     // X position in inertial coordinate system (X positive north)
      	float b_Y;     // Y position in inertial coordinate system (Y positive east)
      	float b_Z;     // Z position in inertial coordinate system (Z positive down)
      
      	float b_Yaw;   // yaw angle (positive right)
      	float b_Pitch; // pitch angle (positive up)
      	float b_Roll;  // roll angle (positive right)

      	//  linear and linear acceleration
      	float b_Vx; 	// X component of velocity in body coordindates (forward)
      	float b_Vy; 	// Y component of velocity in body coordindates (right positive)
      	float b_Vz; 	// Z component of velocity in body coordindates (down positive)
      
      	float b_Ax; 	// acceleration along X axis (forward positive)
      	float b_Ay; 	// acceleration along Y axis (right positive)
      	float b_Az; 	// acceleration along Z axis (down positive)

      	//  Force and Toque
      	float b_Fx; 	// force along X axis (forward positive)
      	float b_Fy; 	// force along Y axis (right positive)
      	float b_Fz; 	// force along Z axis (down positive)

      	float b_Tx; 	// torque along X axis (right roll positive)
      	float b_Ty; 	// torque along Y axis (pitch up positive)
      	float b_Tz; 	// torque along Z axis (yaw right positive)
      
      //  Angular velocity and acceleration
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
      	double b_Beta;  // sideslip angle
      	double b_Thrust;
      	float  b_Altitude;
      
      
      
      // ------------------------------------
      //  Aircraft Declarations
  //    RCSim::DataClasses::AircraftParameters b_AircraftParameters;    // Gets/sets aircraft parameters.

      // ------------------------------------
      // Fields
      
      double b_rhoSurface;
      double b_rhoVerticalSurface;
      double b_airSpeedSq;
      double b_Weight;

    //  Heightmap b_heightmap;

      // ------------------------------------
      // Controls   -  moved to Cntrls data structure
      double b_Throttle; // throttle [-1, 1].
      double b_Elevator; // elevator deflection [-1, 1].
      double b_Ailerons; //  aileron deflection [-1, 1].
      double b_Rudder; // Grudder deflection [-1, 1].

      // ------------------------------------
      // Wind
      Vector3 Wind; // Gets/Sets wind vector [m/s]


      // ------------------------------------
      // constants

      //const double _AirDensity = 1.2041; // density of air (at sea level and standard pressure). [kg/m3]

      /// Gravitational acceleration.
      
      //const double _Gravity = 9.81; // [m/s2]
 
      // ------------------------------------
      // properties

      /// Gets/Sets heightmap.
      
      


      
      
   //   Heightmap b_Heightmap;
      
      // DEBUG properties
      Vector3 DebugPosition;

      void UpdateAirspeed();
      
      void CalculateForces( float, float ); // Calculates total forces on airframe.  time dt,  free stream

      double GetLiftForce(); // Calculates lift force.

      double GetDragForce(); // Calculates drag force.

      double GetSideLiftForce(); // Calculates lift force generated by fuselage and vertical tail.

      double GetSideDragForce(); // Calculates drag force generated by fuselage and vertical tail.

      void CalculateTorques(float elapsedTime); // Calculate torques along different axis of airframe.

      void UpdateCollisions(float elapsedTime);

      bool IsColliding(Vector3 point, Vector3 &normal, float &depth);

      Vector3 MultiplyInertiaInverse(Vector3 vector);

      Vector3 ToDirectX(Vector3 source);

      Vector3 ToModel(Vector3 source);
//    };
//}

#endif
