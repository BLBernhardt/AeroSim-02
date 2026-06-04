//345678901234567890123456789012345678901234567890123456789012345678901234567890
//==============================================================================
//
//       SIMULATION Function,  Flightmodel-810-03
//
//==============================================================================


//#include "aircraftparameters.h"
#include "flightmodel.h"

//=== Added for GLUT 

//#include "../Math_define.h"
#include "../structures.h"

extern struct _Cntrls Cntrls;
extern struct _Aerodyn Aero;
extern struct _TEST test;

//=======================================================
//using namespace FCSim;

//FlightModel::FlightModel( )
//{
//}

//=======================================================
// Structures

typedef struct 
{ 
 	double x, y, z;
} Vec3;

typedef struct 
{ 
 	double w, x, y, z;
} Qtrn4f;

//=======================================================
// Constants

#define PI 		       3.14159265
#define Sign( x )((( x ) < 0 ) ?( -1.0 ) :( 1.0 ))



// Navion Aircraft Parameters

#define MAX_THRUST 	 360.0 	// Thrust in lbs_F
#define THRTL_INIT     0.7
 
#define ELV_TRIM       0.0
#define ELV_MAX_ANG   20.0 
#define AIL_TRIM	   0.0
#define AIL_MAX_ANG   30.0 

#define RUD_TRIM 0.0
#define RUD_MAX_ANG	  20.0

#define ALT_INIT	5000.0
#define V_CRUISE	 210.0 // Cruise speed (ft/s) 170 mph 147.8 knots

// Constants
#define UNITS		"Imperial"
#define G 			 32.1740		// ft/sec^2

#define RHO_0		  0.002377 // Air density (slugs/ft³)
//#define RHO_0 1.2041		//[kg/m3] // density of air( at sea level and standard pressure ) . 
#define RHO_10000	  0.001756
#define RHO 	     RHO_10000

#define FTStoKNOTS	  0.592		// ft/sec = 0.592 knots. 
#define RHO_0_m       1.2041		//[kg/m3] // density of air( at sea level and standard pressure ) . 

//#define dt 0.01 // Time step for integration (s)

// Aircraft Parameters
#define _Iyy 1346.0 // Pitch moment of inertia ( slugs*ft^2 )
#define _Ixx		948.0
#define _Izz	   1967.0

#define WEIGHT	   2750.0
#define MASS		 (WEIGHT/G) // Aircraft weight (lbs/G slugs )
#define _C 4.9 // Mean aerodynamic chord (ft)
#define _S 			184.0 // Wing area (ft²) 
#define _B 			 33.4 // Wing area (ft²) 

#define MAX_DELTA_E 10.0 	// Max elevator deflection (degrees)

//#define CL_0 0.38 
#define CL_0 0.270
#define CL_ALPHA 	  4.44 // Lift coefficient slope due to AoA (per radian)

#define CL_DELTA_E 	  0.335 	// -0.923 Lift coefficient slope due to elevator deflection (per radian) #define CM_DELTA_E 	 	

#define CD_0 0.025 // Drag coefficient 
#define K			  0.061	// Induced Drag Factor

#define CM_0 		  0.0 // Baseline pitching moment coefficient
//#define CM_Q -0.150 // Pitch damping coefficient 
#define CM_Q		 -0.7
#define CM_DELTA_E 	 -0.923 	// Pitching moment slope due to elevator deflection (per radian)
#define CM_ALPHA 	 -0.683 	// Pitching moment slope due to AoA (per radian)

#define Cl_0			0.0		// Roll, Zero-control moment ( typically small or zero in symmetric flight )
#define Cl_DA		 -0.134	// Roll, Aileron effectiveness 9 change in Cl per radian of aileron deflection ) 
#define Cl_P		 -0.410	// Roll, Damping ( change in Cl per unit of roll rate ) 
#define Cl_R		  0.107	// Roll, Yaw-roll coupling ( change in Cl per unti of yaw rate. )

#define CN_0		 -0.0 	// Yaw, sideslip moment, yaw stability
#define CN_b		  0.0907 	// Yaw, sideslip moment, yaw stability
#define CN_p		 -0.0649 // Yaw, roll-rate moment, rikk-yaw coupling 
#define CN_r		 -0.1199 	// Yaw, yaw-rate moment, yaw damping
#define CN_dr		 -0.0805 	// Yaw, rudder deflection moment, rudder effectiveness
#define CN_da		 -0.0504 // Yaw, aileron defection moment, aileron inducted yaw

#define CY_B		 -0.404			// Side Slip Beta
#define CY_DELTA_R 	  0.185		// 
#define CY_r		  0.267
#define CY_p		 -0.145

//========================================================================================
// Quaternion

 extern Quaternion4d Qtrn_Orient_Disp;
 Qtrn4f 	orient;
 
 Quaternion4d Quat_to_Auis( Quaternion4d q1 );
 Qtrn4f quat_from_ang_rates( Vec3 W, double dt );
 void normalize_quat( Qtrn4f *q ); 
 Vec3 rotate_body_to_inrtl( Vec3 body, Qtrn4f q ); 
 void Qtrn_to_Euler_deg( Qtrn4f q, double *Roll_d, double *Pitch_d, double *Yaw_d ); 
 Qtrn4f quat_multiply( Qtrn4f q1, Qtrn4f q2 );
 
//========================================================================================
 
extern float mouse_x, mouse_y;
extern Quaternion4d Qtrn_Orient_Disp;
//=== Added for GLUT 

//=======================================================
//  V			Linear Velocity x,y,z
//	A			Linear Acceleration	 x,y,z
//
//	W			Angular Velocity x,y,z
//	A			Angular Acceleration x,y,z
//	
//	F			Force x,y,z
//	T			Torgue x,y,z
//========================================================


//void FlightModel::Aero_Computations( float dt, int init )
void Aero_Computations( float dt, int init )
{

	if( dt > 1.0f ) return;

	static double 	time;
	
//===========================================================================================================
// Quaternions

 	static Vec3 	W 			= { 0.0, 0.0, 0.0 };
 	static Qtrn4f 	_q 			= { 1, 0, 0, 0 }; // initial orientation	
 	static Vec3 	Vinf_V 		= { 0.0, 0.0, 0.0 };	
 	static Vec3 	position 	= { 0.0, 0.0, 0.0 };	
 	static Quaternion4d OrientationQuat;
 	static Vector3 	V; 	
	
//===========================================================================================================

	static double 	Roll_d, Pitch_d, Yaw_d; // Degrees, P (+) up, Y (+) right, R (+) right ) 
 	static double 	Roll, Pitch, Yaw;		 // Radians
 	
	static double 	Fu, Fv, Fw;	// X, Y, Z 		linear Force (forward, right, down )
	static double 	Au, Av, Aw;	// u_dot, v_dot, w_dot linear accel 	
	static double 	Vu, Vv, Vw;	// u, v, w 	linear Velocity 
	static double 	Px, Py, Pz;	// X, Y, Z 	linear Position 

	static double 	Lm, Mm, Nm;	// L, M, N (xyz) angular moment
	static double 	Ap, Aq, Ar;	// p_dot, q_dot, r_dot (xyz) angular accel
	static double 	Wp, Wq, Wr;	// p, q, r (xyz) angular rate
	static double 	Pp, Pq, Pr;	// Roll (x), Pitch (y), Yaw (z) angular position
 	static double 	Vinf;			// ft/sec

	static double 	Alpha, Alpha2;	// angle of attack 
	static double 	Beta;	 		// sideslip angle 
	static double 	Lift, Drag; 
	static double 	Altitude_ft = ALT_INIT; 	// ft

	Vector3 		Wind; 			// wind vector [m/s] 

 	static double 	Weight, Thrust; 

	static double 	Altitude_m;		// 1 ft = 3.28094 meters
	
 	static double 	delta_e_deg; 	// Elevator deflection (degrees)
 	static double 	delta_e; 		// Elevator deflection (radians)
 	static double 	delta_e_trim;
 
 	static double 	delta_a_deg;
 	static double 	delta_a;
 
 	static double 	delta_r_deg;
 	static double 	delta_r;
 
 	double 			sideLift, sideDrag;
 	float 			qS, qSc, qSb;
 	double 			Vinf_Sq;
 
//================== Init ====================================================================================================
 	if( init )
 	{
		if( strcmp( UNITS, "Metric" ))
		{	
			printf("1] UNITS set to EMPERICAL\n");
		}
		else if( strcmp( UNITS, "Imperial" ))
		{	
			printf("2] UNITS set to EMPERICAL\n");
		}
		else 
		{
			printf(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Units Not defined \n <<<<<<<<<<<<<<<<<<<<<<<<< ");	
		}
		
		time = -1.0;
 	
 		Weight 				= WEIGHT;	 // Weight lbs : mass (lbs/G slugs )
 //		OrientationQuat 	= OrientationQuat.RotationYawPitchRoll( 0.0f, 0.0f, 0.0f );
  		OrientationQuat 	= Quaternion_RotationYawPitchRoll( 0.0f, 0.0f, 0.0f );		

		Cntrls.Throttle_Cmd = THRTL_INIT;
	
		Vv					= 0.0;
		Vw 					= 0.0;
		Vu 					= V_CRUISE;

		Altitude_m 			= ALT_INIT / 3.28084; 			// 1 ft = 3.28094 meters
		Altitude_ft 		= ALT_INIT; 
	
		Cntrls.Throttle_Cmd = THRTL_INIT; 
	
		Vinf_Sq					= 0.0;
	
	}


//================== End Init ================================================================================================

 	time += dt;

//================ Controls ================================================================================================

 	// Cntrls.Elevator_Cmd 		// Elevator deflection [- 1, 1]. 	 
	// Cntrls.Aileron_Cmd 		// Aileron deflection [- 1, 1]. 	 
    //	double Rud_Defl; 		// Eudder deflection [- 1, 1]. 	 	 
 	// Cntrls.Throttle_Cmd 		// Throttle setting [ 0, 1]. 

	delta_e_deg  = -(Cntrls.Elevator_Cmd * ELV_MAX_ANG ) ; 		// Pitch stick y axis range -1.0 to 1.0 
 	delta_e_trim = ELV_TRIM;
 	delta_e 	 = (delta_e_deg + delta_e_trim) * DEGtoRAD;
 
	delta_a_deg  = (Cntrls.Aileron_Cmd * AIL_MAX_ANG ) / 5.0 ; 	// Roll stick x axis range -1.0 to 1.0 
 	delta_a 	 = delta_a_deg * DEGtoRAD;	

	delta_r_deg  = (Cntrls.Rudder_Cmd * RUD_MAX_ANG ); 			// Roll stick x axis range -1.0 to 1.0 
 	delta_r 	 = delta_r_deg * DEGtoRAD;	
 
	Thrust = Cntrls.Throttle_Cmd * MAX_THRUST; 					// Throttle Command setting [ 0, 1]. 		 

 	Weight = WEIGHT; 
 	 
 //------- Test Code ------------------------------------------------ 
 	#if 0	 	
 	 	printf("4] Thrst %3.2f Elev %3.2f %3.2f Pitch %7.4f \n", 	Thrust, delta_e_deg, Cntrls.Elevator_Cmd, Pitch );
 	#endif	 
 	#if 0	 	
 	 	printf("5] Aileron %3.2f da deg %3.2f da rad %7.4f \n\n", 	delta_a_deg, delta_a_deg, delta_a );
 	#endif 

//================== End Controls ============================================================================================

//================== Airspeed, Alpha, Beta, Flight Path ======================================================================
// Update the airspeed 
	
	
 	if( abs( Vu ) < 0.001 )			// prevent devide by zero
 	{
 		Alpha = 0.0;
 		Beta  = 0.0;
 	}
 	else
 	{
 		Alpha = atan2(  Vw, Vu ); 	// geometry - rise over run 
 		Beta  = atan2( -Vv, Vu );
 	}

	Vinf    = sqrt( Vu * Vu + Vw * Vw );	
	Vinf_Sq	= Vu * Vu + Vv * Vv + Vw * Vw;
	Vinf 	= sqrt( Vinf_Sq);

	Aero.Alpha = Alpha * RADtoDEG; 
	Aero.Beta  = Beta * RADtoDEG;

	sideLift = 0.0;
	sideDrag = 0.0;

 	qS  = 0.5*RHO*Vinf*Vinf*_S;
 	qSc = qS * _C;
 	qSb = qS * _B;

//============================================================================================================================
	
	delta_e_trim = ELV_TRIM;
	
	float Cmo 	= CM_0;
 	float Cmq 	= CM_Q; 
	float Cmde 	= CM_DELTA_E;
	float Cma 	= CM_ALPHA;
 	
 	float Clo 	= Cl_0;		// Roll, Zero-control moment ( typically small or zero in symmetric flight )
 	float Clda 	= Cl_DA;	// Roll, Aileron effectiveness 9 change in Cl per radian of aileron deflection )
 	float Clp 	= Cl_P;		// Roll, Damping ( change in Cl per unit of roll rate ) 
 	float Clr 	= Cl_R;		// Roll, Yaw-roll coupling ( change in Cl per unti of yaw rate. )
 	
 	float Cno 	= 0.0;
	float Cnb 	= CN_b;		// Yaw, sideslip moment, yaw stability
	float Cnp 	= CN_p;		// Yaw, roll-rate moment, rikk-yaw coupling 
	float Cnr 	= CN_r;		// Yaw, yaw-rate moment, yaw damping
	float Cndr 	= CN_dr;		// Yaw, rudder deflection moment, rudder effectiveness
	float Cnda 	= CN_da;	 // Yaw, aileron defection moment, aileron inducted yaw 	

//=======================================================================================================================
// Moments - rotation

		// X axis
		Lm = qSb * ( ( Clo ) + ( Clda * delta_a ) + ( Clp*Wp*_B/(2.0*Vinf)) + ( Clr*Wr*_B/(2.0*Vinf)));
		Ap = ( Lm / _Ixx );	// calc roll rate radians/sec. (Force / Moment_Inertia) * time 
		Wp += Ap * dt;	 
		
		// Y axis
		Mm = qSc * ( ( Cmde * delta_e ) + (Cma * Alpha) + ( Cmq * Wq ) ); 
 		Aq = ( Mm / _Iyy );	// calc pitch rate radians/sec. (Force / Moment_Inertia) * time 	
 		Wq += Aq * dt;		

 		// Z axis 
 		Nm = qSb * ( Cno + ( Cnb * Beta ) + ( Cnp * Wp*_B/(2.0*Vinf)) + ( Cnr * Wr*_B/(2.0*Vinf) + ( Cndr * delta_r ) ));
 		Ar = ( Nm / _Izz );		// calc yaw rate radians/sec. (Force / Moment_Inertia) * time
		Wr += Ar * dt; 

//=======================================================================================================================	
// Forces - linear
	
	Weight = WEIGHT;	 
 	float CLo 	= CL_0;
 	float CLa 	= CL_ALPHA; 	 	
 	float CDo 	= CD_0;
	float CL, Cd, Cy;

	float Cyb	= CY_B;
	float Cydr	= CY_DELTA_R;
	float Cyp	= CY_p;
	float Cyr 	= CY_r;

		CL = ( CLo + ( CLa * Alpha ));
		Cd = ( CDo + ( K*CL*CL ));
		
		Lift = qS * CL;
		Drag = qS * Cd;

//===============================================================	
		// X Axis
		Fu = ( float )( ( Lift * sin( Alpha )) - ( Drag * cos( Alpha )) + ( Thrust ) - ( Weight * sin( Pitch ))); //( sideLift * sin( Beta )) - ( sideDrag * cos( Beta )) -
		Au = ( float )( Fu / MASS );	
 		Vu += Au * dt;	

		// Y Axis
		Cy =   Cyb * Beta
			 + Cydr * delta_r
			 + Cyp * ( Wp * Vw / (2.0 * Vinf ))
			 + Cyr * ( Wr * Vw / (2.0 * Vinf ));

		Fv = qS * Cy;
	 	Av = (Fv / MASS ) + Wr * Vu - Wp * Vw + G * cos(Pitch) * sin(Roll);		
		Vv += Av * dt;
	
	
		// Z axis	
		Fw = ( float )(( Lift * cos( Alpha )) - ( Drag * sin( Alpha )) - ( Weight * cos( Roll ) * cos( Pitch ))); // Equation with Z axis up (+);
		Fw = -Fw; // Flip for Z axis sign convention, right hand rule 
		Aw = ( float )( Fw / MASS );
 		Vw += Aw * dt;

//=======================================================================================================================	
		W.x = Wp;
		W.y = Wq;
		W.z = Wr;

		#if 1
		int QTRN_SW = 0;

#if 0
		if( QTRN_SW )
		{

	 		Quaternion newOrientation
	 		( 
				OrientationQuat.x +( OrientationQuat.w * Wp + OrientationQuat.y * Wr - OrientationQuat.z * Wq ) * 0.5f * dt,
				OrientationQuat.y +( OrientationQuat.w * Wq + OrientationQuat.z * Wp - OrientationQuat.x * Wr ) * 0.5f * dt,
				OrientationQuat.z +( OrientationQuat.w * Wr + OrientationQuat.x * Wq - OrientationQuat.y * Wp ) * 0.5f * dt,
				OrientationQuat.w -( OrientationQuat.x * Wp + OrientationQuat.y * Wq + OrientationQuat.z * Wr ) * 0.5f * dt
			);

	 		newOrientation.Normalize( );
 
	 		OrientationQuat = newOrientation;
 
	 		Vector3 ypr = OrientationQuat_ToEulerAngles( );

	 		Roll 	= ypr.x;
	 		Pitch 	= ypr.y;
	 		Yaw 	= ypr.z;

	 		Qtrn_Orient_Disp.x = OrientationQuat.x;
	 		Qtrn_Orient_Disp.y = OrientationQuat.y;
	 		Qtrn_Orient_Disp.z = OrientationQuat.z;
	 		Qtrn_Orient_Disp.w = OrientationQuat.w;
		}
#endif

		if( QTRN_SW )
		{
	 		Quaternion4d newOrientation;
	 		Quaternion_Init(&newOrientation);
	 		
	 		double half_dt = 0.5 * dt;
	 		
	 		( 
				newOrientation.x +( OrientationQuat.w * Wp + OrientationQuat.y * Wr - OrientationQuat.z * Wq ) * 0.5f * dt,
				newOrientation.y +( OrientationQuat.w * Wq + OrientationQuat.z * Wp - OrientationQuat.x * Wr ) * 0.5f * dt,
				newOrientation.z +( OrientationQuat.w * Wr + OrientationQuat.x * Wq - OrientationQuat.y * Wp ) * 0.5f * dt,
				newOrientation.w -( OrientationQuat.x * Wp + OrientationQuat.y * Wq + OrientationQuat.z * Wr ) * 0.5f * dt
			);

	 		Quaternion_Normalize( &newOrientation );
 
	 		Quaternion_Copy( &OrientationQuat, &newOrientation );
 
	 		Vector3 ypr = Quaternion_ToEulerAngles( &OrientationQuat );

	 		Roll 	= ypr.x;
	 		Pitch 	= ypr.y;
	 		Yaw 	= ypr.z;

	 		Qtrn_Orient_Disp.x = OrientationQuat.x;
	 		Qtrn_Orient_Disp.y = OrientationQuat.y;
	 		Qtrn_Orient_Disp.z = OrientationQuat.z;
	 		Qtrn_Orient_Disp.w = OrientationQuat.w;
		}
		else
		{
	
 			//==================================================================
 			// Body to Intertial  transform
 			Qtrn4f d_q;
 			Vec3 Vi_inertial;
 			
 			// Quaternion process,  next 5 lines
 			d_q = quat_from_ang_rates( W, dt ); 	
 			_q  = quat_multiply( _q, d_q );
 			normalize_quat( &_q );
  			Vi_inertial = rotate_body_to_inrtl( Vinf_V, _q ); 		
	 		Qtrn_to_Euler_deg( _q, &Roll_d, &Pitch_d, &Yaw_d );
 
 
 			Vinf_V.x = Vu;
 			Vinf_V.y = Vv;
 
 			//==================================================================
 			// 6DOF inertial solution 		

	 		position.x += Vi_inertial.x * dt;
	 		position.y += Vi_inertial.y * dt;
	 		position.z -= Vi_inertial.z * dt; // 
	 		
	 		Aero.Position_inertial_X_ft = position.x;
			Aero.Position_inertial_Y_ft = position.y;
			Aero.Position_inertial_Z_ft = position.z;

	 		Pitch 	= Pitch_d * DEGtoRAD;
	 		Roll 	= Roll_d * DEGtoRAD; 
	 		Yaw 	= Yaw_d * DEGtoRAD; 	
		
		 	Qtrn_Orient_Disp.x = _q.x;
	 		Qtrn_Orient_Disp.y = _q.y;
	 		Qtrn_Orient_Disp.z = _q.z;
	 		Qtrn_Orient_Disp.w = _q.w;
	
	}

#endif

#if 0
	{
 		Weight = MASS*G;

 		printf( "\033[7F" );    //  controls cursor on terminal for non scrolling and back up 7 lines.
 		
 		printf( "10]-------------------------------------------------------------------------------------------------------------------------------------\n" );
 		printf( "| Time s | Vinf Ang | Vin | Alt | Elv Ang | Cm | q | Pitch-Pitch | | | \n" );
 		printf( "| %8.2f | %8.2f | %8.2f | %8.2f | %8.2f | %8.2f | %8.2f | %9.3f | | |\n", 
 		time, flight_path_angle, Vinf, altitude, delta_e * RADtoDEG , CM_DELTA_E * delta_e * RADtoDEG, q * RADtoDEG, Pitch * RADtoDEG ); 
 		printf( "-------------------------------------------------------------------------------------------------------------------------------------\n" );
		printf( "| Time s | Vu | Vw | Pitch | Vif | Alpha | Weight | Lift | Drag | Thrust | Thrtl_Set | \n" );
 		printf( "| %8.2f | %8.2f | %8.2f | %8.2f | %8.2f | %8.2f | %8.2f | %9.2f | %9.2f | %9.2f | %3.2f |\n", time, Vu, Vw, pitch_d, Vinf, Alpha * RADtoDEG, Weight, lift, drag, thrust, Thrtl_Set );
 		printf( "11]-------------------------------------------------------------------------------------------------------------------------------------\n" );

	} 
#endif

//=================================================================================================================
// PFD Interface 

 		Aero.Pitch_d 	= Pitch * RADtoDEG;
 		Aero.Roll_d 	= Roll 	* RADtoDEG;
 		Aero.Yaw_d 		= Yaw 	* RADtoDEG; 
	
		if( strcmp( UNITS, "Imperial" ) == 0 )
		{	
			Altitude_ft += Vw * dt;
 			Aero.RateOfClimb_ft_sec = Vw * 60.0 ;
	
			if( Altitude_ft > 50000.0 ) 	Altitude_ft = 50000.0 ;
			if( Altitude_ft < 0.0 ) 		Altitude_ft = 0.0 ;		 
	 
			Aero.Altitude_AGL_feet = Altitude_ft; 
			Aero.AirSpeed_knots = Vu * 0.592; // ft/sec = 0.592 knots. 
		}
		else printf( "12]>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Units Not defined \n <<<<<<<<<<<<<<<<<<<<<<<<< " );	

// End - PFD Int_erface 
//=================================================================================================================


}
// End of Calc Aero routine
//========================================================================================================================
//  Retained needed C++ code

//Vector3 FlightModel::getAttitude( )
Vector3 getAttitude( )
{
 //	Vector3 result( b_Roll, b_Pitch, b_Yaw );

  Vector3 result;
	result.x = b_Roll;
	result.y = b_Pitch;
	result.z = b_Yaw;


 	return result;
}

//Vector3 FlightModel::getPosition( )
Vector3 getPosition( )
{
//Vector3 result( b_X, b_Y, b_Z );
// Vector3 result( 1.0, 2.0, 3.0 );

  Vector3 result;
	result.x = b_X;
	result.y = b_Y;
	result.z = b_Z;
	
 	return result;
}

//bool FlightModel::Load( string filename )

#if 0
bool Load( string filename )
{
 bool result = true;

 	b_AircraftParameters.ReadParameters( filename );

 	return result;
}
#endif

//=================================================================================================================
// ChatGTP Quaternion Code

// Rotate body Vel to inrt frame
Vec3 T_Body_to_Inrt_Vel( const Vec3 &V_body, const Qtrn4f &q ) 
{
	Vec3 return_V;
	 float x = q.x, y = q.y, z = q.z, w = q.w;

	 float R11 = 1 - 2 * ( y * y + z * z );
	 float R12 = 2     * ( x * y - z * w );
	 float R13 = 2     * ( x * z + y * w );

	 float R21 = 2     * ( x * y + z * w );
	 float R22 = 1 - 2 * ( x * x + z * z );
	 float R23 = 2     * ( y * z - x * w );

	 float R31 = 2     * ( x * z - y * w );
	 float R32 = 2     * ( y * z + x * w );
	 float R33 = 1 - 2 * ( x * x + y * y );

 	return return_V = 
 	{ 
 		R11 * V_body.x + R12 * V_body.y + R13 * V_body.z,
 		R21 * V_body.x + R22 * V_body.y + R23 * V_body.z,
 		R31 * V_body.x + R32 * V_body.y + R33 * V_body.z
 	};
}

// Compute Heading Ang_le ( _radians )
float Heading( const Qtrn4f &q ) 
{
	 float x = q.x, y = q.y, z = q.z, w = q.w;

	 // Rotate body x-axis ( 1,0,0 ) Int_o inrt frame
	 float fx = 1 - 2 * ( y*y + z*z );
	 float fz = 2 * ( x*z - w*y );

	 float Heading_rad = std::atan2( fx, fz ); // atan2( east, north )
	 
	 return Heading_rad;

}

// Create delta quaternion from angular Vi
Qtrn4f quat_from_ang_rates( Vec3 W, double dt ) 
{ 
 	double mag = sqrt( W.x * W.x + W.y * W.y + W.z * W.z );
 	
 	if( mag == 0.0 ) return( Qtrn4f ){ 1, 0, 0, 0 };

 	double half_angle = 0.5 * mag * dt;
 	double sin_half = sin( half_angle );
 	double cos_half = cos( half_angle );

 	return( Qtrn4f )
 	{ 
 		cos_half,
 		sin_half * W.x / mag,
 		sin_half * W.y / mag,
 		sin_half * W.z / mag
 	 };
 }
 
// Quaternion multiplication
Qtrn4f quat_multiply( Qtrn4f q1, Qtrn4f q2 ) 
{ 
 	Qtrn4f r;
 	r.w = q1.w * q2.w - q1.x * q2.x - q1.y * q2.y - q1.z * q2.z;
 	r.x = q1.w * q2.x + q1.x * q2.w + q1.y * q2.z - q1.z * q2.y;
 	r.y = q1.w * q2.y - q1.x * q2.z + q1.y * q2.w + q1.z * q2.x;
 	r.z = q1.w * q2.z + q1.x * q2.y - q1.y * q2.x + q1.z * q2.w;
 	return r;
}

// Normalize quaternion
void normalize_quat( Qtrn4f *q ) 
{ 
 	double mag = sqrt( q->w * q->w + q->x * q->x + q->y * q->y + q->z * q->z );
 	if( mag > 0.0 )
 	{
 		q->w /= mag;
 		q->x /= mag;
 		q->y /= mag;
 		q->z /= mag;
 	}
 	else printf("14] Error - normalize_quat() - something wrong with quaternion - divide by zero, mag = 0.0 \n"); 
}

// Rotate body vector to inertial frame using quaternion
Vec3 rotate_body_to_inrtl( Vec3 body, Qtrn4f q ) 
{ 
 	Qtrn4f p = { 0, body.x, body.y, body.z };
 	Qtrn4f q_conj = { q.w, -q.x, -q.y, -q.z };
 	Qtrn4f rotated = quat_multiply( quat_multiply( q, p ), q_conj );
 	return( Vec3 ){ rotated.x, rotated.y, rotated.z };
}

// Extract Euler angles from quaternion( roll, pitch, yaw in degrees )
void Qtrn_to_Euler_deg( Qtrn4f q, double *Roll_d, double *Pitch_d, double *Yaw_d ) 
{ 
 	// Roll( X-axis rotation )
 	double sinr_cosp = 2.0 * ( q.w * q.x + q.y * q.z );
 	double cosr_cosp = 1.0 - 2.0 * ( q.x * q.x + q.y * q.y );
 	double roll = atan2( sinr_cosp, cosr_cosp );

 	// Pitch( Y-axis rotation )
 	double sinp = 2.0 * ( q.w * q.y - q.z * q.x );
 	double pitch;
 	if( fabs( sinp ) >= 1.0 )
 		pitch = copysign( M_PI / 2.0, sinp );
 	else
 		pitch = asin( sinp );

 	// Yaw( Z-axis rotation )
 	double siny_cosp = 2.0 * ( q.w * q.z + q.x * q.y );
 	double cosy_cosp = 1.0 - 2.0 * ( q.y * q.y + q.z * q.z );
 	double yaw = atan2( siny_cosp, cosy_cosp );

 	// Convert to Degrees
 	*Roll_d 	= roll * 180.0 / M_PI;
 	
 	*Pitch_d 	= pitch * 180.0 / M_PI;
 	
 	*Yaw_d 		= yaw * 180.0 / M_PI;
 	if( *Yaw_d < 0.0 ) * Yaw_d += 360.0;
 }
 
// End - ChatGTP Quaternion Code
//================================================================================================================= 
 
// EOF
//=================================================================================================================





























