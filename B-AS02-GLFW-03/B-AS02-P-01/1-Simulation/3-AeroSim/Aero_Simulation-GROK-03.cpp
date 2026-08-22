//==============================================================================
//
// AeroSimulation_GROK_03.cpp
// self-contained GROK math version
//
//==============================================================================

#include <math.h>
#include <stdio.h>
#include <string.h>

#include "../structures.h"
#include "../IPC_UDP_Common.h"

#include "../2-MODEL/aero_model.h"
#include "../2-MODEL/coeff_table.h"

#include "Aero_Simulation.h"


void Aero_Simulation( float dt, Aerodyn_t* Aero_D );
Quaternion4 Qtrn_Orient_Disp; 

//==============================================================================
// SWITCHES ( last known stable set from testing ) Temp Code Test Switches 
//==============================================================================
#define FIX_QUAT_HELPERS 1 // always needed
#define FIX_FULL_BODY_VEL 1
#define FIX_YAW_PARENTHESES 1
#define FIX_DENSITY_ALT 1
#define FIX_ALTITUDE_CONSISTENT 1
#define FIX_CMQ_NONDIM 0 // leave off until CM_Q is retuned
#define FIX_ALPHA_BETA 0 // leave off for now

//==============================================================================
// Constants
//==============================================================================
#define MAX_THRUST 360.0
#define THRTL_INIT 0.7
#define ELV_TRIM 0.0
#define ELV_MAX_ANG 20.0
#define AIL_MAX_ANG 30.0
#define RUD_MAX_ANG 20.0
#define ALT_INIT 5000.0
#define V_CRUISE 210.0

#define UNITS "Imperial"
#define G 32.1740
#define RHO_0 0.002377
#define RHO_10000 0.001756
#define RHO RHO_10000

#define _Ixx 948.0
#define _Iyy 1346.0
#define _Izz 1967.0
#define WEIGHT 2750.0
#define MASS ( WEIGHT / G )
#define _C 4.9
#define _S 184.0
#define _B 33.4

#define CL_0 0.270
#define CL_ALPHA 4.44
#define CD_0 0.025
#define K 0.061
#define CM_0 0.0
#define CM_Q -0.7
#define CM_DELTA_E -0.923
#define CM_ALPHA -0.683
#define Cl_0 0.0
#define Cl_DA -0.134
#define Cl_P -0.410
#define Cl_R 0.107
#define CN_0 0.0
#define CN_b 0.0907
#define CN_p -0.0649
#define CN_r -0.1199
#define CN_dr -0.0805
#define CY_B -0.404
#define CY_DELTA_R 0.185
#define CY_p -0.145
#define CY_r 0.267

//==============================================================================
// GROK math types
//==============================================================================
typedef struct { double x, y, z; } GROK_Vec3;
typedef struct { double w, x, y, z; } GROK_Quat;

//==============================================================================
// GROK Quaternion / Vector routines
//==============================================================================
static GROK_Quat GROK_quat_identity( void )
{
 GROK_Quat q = { 1.0, 0.0, 0.0, 0.0 };
 return q;
}

static GROK_Quat GROK_quat_from_ang_rates( GROK_Vec3 W, double dt )
{
 double mag = sqrt( W.x*W.x + W.y*W.y + W.z*W.z );
 if( mag < 1.0e-12 )
 return GROK_quat_identity( );

 double half = 0.5 * mag * dt;
 double s = sin( half );
 double c = cos( half );

 GROK_Quat q;
 q.w = c;
 q.x = s * W.x / mag;
 q.y = s * W.y / mag;
 q.z = s * W.z / mag;
 return q;
}

static GROK_Quat GROK_quat_multiply( GROK_Quat a, GROK_Quat b )
{
 GROK_Quat r;
 r.w = a.w*b.w - a.x*b.x - a.y*b.y - a.z*b.z;
 r.x = a.w*b.x + a.x*b.w + a.y*b.z - a.z*b.y;
 r.y = a.w*b.y - a.x*b.z + a.y*b.w + a.z*b.x;
 r.z = a.w*b.z + a.x*b.y - a.y*b.x + a.z*b.w;
 return r;
}

static void GROK_quat_normalize( GROK_Quat *q )
{
 double m = sqrt( q->w*q->w + q->x*q->x + q->y*q->y + q->z*q->z );
 if( m > 1.0e-12 ) {
 q->w /= m;
 q->x /= m;
 q->y /= m;
 q->z /= m;
 }
}

static GROK_Vec3 GROK_quat_rotate_body_to_inertial( GROK_Vec3 body, GROK_Quat q )
{
 GROK_Quat p = { 0.0, body.x, body.y, body.z };
 GROK_Quat qc = { q.w, -q.x, -q.y, -q.z }; // conjugate
 GROK_Quat t = GROK_quat_multiply( q, p );
 GROK_Quat r = GROK_quat_multiply( t, qc );

 GROK_Vec3 out = { r.x, r.y, r.z };
 return out;
}

static void GROK_quat_to_euler_deg( GROK_Quat q, double *roll_d, double *pitch_d, double *yaw_d )
{
 // Roll( X )
 double sinr_cosp = 2.0 *( q.w * q.x + q.y * q.z );
 double cosr_cosp = 1.0 - 2.0 *( q.x * q.x + q.y * q.y );
 double roll = atan2( sinr_cosp, cosr_cosp );

 // Pitch( Y )
 double sinp = 2.0 *( q.w * q.y - q.z * q.x );
 double pitch;
 if( fabs( sinp ) >= 1.0 )
 pitch = copysign( PI / 2.0, sinp );
 else
 pitch = asin( sinp );

 // Yaw( Z )
 double siny_cosp = 2.0 *( q.w * q.z + q.x * q.y );
 double cosy_cosp = 1.0 - 2.0 *( q.y * q.y + q.z * q.z );
 double yaw = atan2( siny_cosp, cosy_cosp );

 *roll_d = roll * RADtoDEG;
 *pitch_d = pitch * RADtoDEG;
 *yaw_d = yaw * RADtoDEG;
 if( *yaw_d < 0.0 ) *yaw_d += 360.0;
}

// Replacement for the old Quaternion_RotationYawPitchRoll
#if 0
static GROK_Quat GROK_quat_from_euler_ypr( double yaw_rad,
 double pitch_rad,
 double roll_rad )
{
 double cy = cos( yaw_rad * 0.5 );
 double sy = sin( yaw_rad * 0.5 );
 double cp = cos( pitch_rad * 0.5 );
 double sp = sin( pitch_rad * 0.5 );
 double cr = cos( roll_rad * 0.5 );
 double sr = sin( roll_rad * 0.5 );

 GROK_Quat q;
 q.w = cr * cp * cy + sr * sp * sy;
 q.x = sr * cp * cy - cr * sp * sy;
 q.y = cr * sp * cy + sr * cp * sy;
 q.z = cr * cp * sy - sr * sp * cy;
 return q;
}
#endif
//==============================================================================
// Density helper
//==============================================================================
static double GROK_get_rho( double alt_ft )
{
#if FIX_DENSITY_ALT
 if( alt_ft < 0.0 ) alt_ft = 0.0;
 if( alt_ft > 20000.0 ) alt_ft = 20000.0;
 double t = alt_ft / 10000.0;
 if( t > 1.0 ) t = 1.0;
 return RHO_0 +( RHO_10000 - RHO_0 ) * t;
#else
 return RHO;
#endif
}

//==============================================================================
// Main computation
//==============================================================================
void Aero_Computations( float dt, Aerodyn_t* Aero_D, Cntrls_t* CTRL )
{
 if( dt > 1.0f || dt <= 0.0f ) return;

 // Persistent state
 static double time = 0.0;
 static int first_pass = 1;

 static GROK_Vec3 W = {0.0, 0.0, 0.0};
 static GROK_Quat _q = {1.0, 0.0, 0.0, 0.0};
 static GROK_Vec3 position = {0.0, 0.0, 0.0};
// static GROK_Vec3 Vinf_V = {0.0, 0.0, 0.0};

 static double Vu = 0.0, Vv = 0.0, Vw = 0.0;
 static double Wp = 0.0, Wq = 0.0, Wr = 0.0;
 static double Roll = 0.0, Pitch = 0.0;// Yaw = 0.0;
 static double Roll_d = 0.0, Pitch_d = 0.0, Yaw_d = 0.0;
 static double Altitude_ft = ALT_INIT;

 static double Lift = 0.0, Drag = 0.0;
 static double Fu = 0.0, Fv = 0.0, Fw = 0.0;

 //------------------------------------------------------------------
 // Initialization
 //------------------------------------------------------------------
	if( first_pass ) 
	{
		first_pass = 0;
		Aero_D->Units = FPS;

		printf( "Aero_Computations( GROK self-contained ): Units = FPS\n" );

		time = 0.0;
		Vu = V_CRUISE;
		Vv = 0.0;
		Vw = 0.0;
		Wp = Wq = Wr = 0.0;
		Altitude_ft = ALT_INIT;
		position.x = position.y = position.z = 0.0;

		_q = GROK_quat_identity( );

		CTRL->Throttle_Cmd = THRTL_INIT;
	}

	time += dt;

	//------------------------------------------------------------------
	// Controls
	//------------------------------------------------------------------
	double delta_e_deg = -( CTRL->Elevator_Cmd * ELV_MAX_ANG );
	double delta_e =( delta_e_deg + ELV_TRIM ) * DEGtoRAD;

	double delta_a_deg =( CTRL->Aileron_Cmd * AIL_MAX_ANG ) / 5.0;
	double delta_a = delta_a_deg * DEGtoRAD;

	double delta_r_deg =( CTRL->Rudder_Cmd * RUD_MAX_ANG );
	double delta_r = delta_r_deg * DEGtoRAD;

	double Thrust = CTRL->Throttle_Cmd * MAX_THRUST;
	double Weight = WEIGHT;

	//------------------------------------------------------------------
	// Air data
	//------------------------------------------------------------------
	double Vinf_Sq = Vu*Vu + Vv*Vv + Vw*Vw;
	double Vinf = sqrt( Vinf_Sq );
	if( Vinf < 1.0 ) Vinf = 1.0;

#if FIX_ALPHA_BETA
	double Alpha = atan2( Vw, Vu );
	double Beta = atan2( Vv, sqrt( Vu*Vu + Vw*Vw ) );
#else
	double Alpha = atan2( Vw, Vu );
	double Beta = atan2( -Vv, Vu );
#endif

	Aero_D->Alpha =( float )( Alpha * RADtoDEG );
	Aero_D->Beta =( float )( Beta * RADtoDEG );

	double rho = GROK_get_rho( Altitude_ft );
	double qS = 0.5 * rho * Vinf_Sq * _S;
	double qSc = qS * _C;
	double qSb = qS * _B;

 //------------------------------------------------------------------
 // Moments
 //------------------------------------------------------------------
 // Roll ( R wing down )-----------------------------------------------------
	double Lm = qSb *( Cl_0
			+ Cl_DA * delta_a
			+ Cl_P *( Wp * _B /( 2.0 * Vinf ) )
			+ Cl_R *( Wr * _B /( 2.0 * Vinf ) ) );
	double Ap = Lm / _Ixx;
	Wp += Ap * dt;

 // Pitch ( + nose up )-----------------------------------------------------
#if FIX_CMQ_NONDIM
	double Mm = qSc *( CM_0
			+ CM_ALPHA * Alpha
			+ CM_Q *( Wq * _C /( 2.0 * Vinf ) )
			+ CM_DELTA_E * delta_e );
#else
	double Mm = qSc *( CM_DELTA_E * delta_e
			+ CM_ALPHA * Alpha
			+ CM_Q * Wq ); // original raw-rate form
#endif
	double Aq = Mm / _Iyy;
	Wq += Aq * dt;

 // Yaw ( nose right )-----------------------------------------------------
#if FIX_YAW_PARENTHESES
	double Nm = qSb *( CN_0
			+ CN_b * Beta
			+ CN_p *( Wp * _B /( 2.0 * Vinf ) )
			+ CN_r *( Wr * _B /( 2.0 * Vinf ) )
			+ CN_dr * delta_r );
#else
 double Nm = qSb *( CN_0
			+( CN_b * Beta )
			+( CN_p * Wp * _B /( 2.0 * Vinf ) )
			+( CN_r * Wr * _B /( 2.0 * Vinf ) +( CN_dr * delta_r ) ) );
#endif
	double Ar = Nm / _Izz;
	Wr += Ar * dt;

	W.x = Wp; W.y = Wq; W.z = Wr;

 //------------------------------------------------------------------
 // Forces
 //------------------------------------------------------------------
	double CL = CL_0 + CL_ALPHA * Alpha;
	double Cd = CD_0 + K * CL * CL;
	Lift = qS * CL;
	Drag = qS * Cd;

 // +X( forward )-----------------------------------------------------
	Fu =( Lift * sin( Alpha ) - Drag * cos( Alpha ) ) + Thrust
			- Weight * sin( Pitch );
 
	double Au = Fu / MASS;
	Vu += Au * dt;

 // +Y( right )-----------------------------------------------------
	double Cy = CY_B * Beta
			+ CY_DELTA_R * delta_r
			+ CY_p *( Wp * _B /( 2.0 * Vinf ) )
			+ CY_r *( Wr * _B /( 2.0 * Vinf ) );
	Fv = qS * Cy;
	double Av = Fv / MASS + Wr * Vu - Wp * Vw
			+ G * cos( Pitch ) * sin( Roll );
	Vv += Av * dt;

 // +Z( down )-----------------------------------------------------
	Fw =( Lift * cos( Alpha ) - Drag * sin( Alpha ) )
			- Weight * cos( Roll ) * cos( Pitch );
			
	Fw = -Fw; // original sign flip retained
	double Aw = Fw / MASS;
	Vw += Aw * dt;

 //------------------------------------------------------------------
 // Orientation + Position( GROK quaternion path )
 //------------------------------------------------------------------
#if FIX_QUAT_HELPERS
	GROK_Quat d_q = GROK_quat_from_ang_rates( W, dt );
	_q = GROK_quat_multiply( _q, d_q );
	GROK_quat_normalize( &_q );

#if FIX_FULL_BODY_VEL
	GROK_Vec3 V_body = { Vu, Vv, Vw };
	GROK_Vec3 Vi = GROK_quat_rotate_body_to_inertial( V_body, _q );
#else
	GROK_Vec3 Vi = GROK_quat_rotate_body_to_inertial( Vinf_V, _q );
	Vinf_V.x = Vu;
	Vinf_V.y = Vv;
#endif

	GROK_quat_to_euler_deg( _q, &Roll_d, &Pitch_d, &Yaw_d );

	position.x += Vi.x * dt;
	position.y += Vi.y * dt;
	position.z -= Vi.z * dt;

	Aero_D->Position_inertial_X_ft =( float )position.x;
	Aero_D->Position_inertial_Y_ft =( float )position.y;
	Aero_D->Position_inertial_Z_ft =( float )position.z;

	Pitch = Pitch_d * DEGtoRAD;
	Roll = Roll_d * DEGtoRAD;
// Yaw = Yaw_d * DEGtoRAD;

 // Expose quaternion for display( assumes Qtrn_Orient_Disp still exists )
	Qtrn_Orient_Disp.w =( float )_q.w;
	Qtrn_Orient_Disp.x =( float )_q.x;
	Qtrn_Orient_Disp.y =( float )_q.y;
	Qtrn_Orient_Disp.z =( float )_q.z;
#endif

 //------------------------------------------------------------------
 // Correct G-meter( normal load factor )
 // Uses aero + thrust forces only( excludes gravity )
 //------------------------------------------------------------------
 // double Fu_aero =( Lift * sin( Alpha ) - Drag * cos( Alpha ) ) + Thrust;
 // double Fv_aero = Fv;
	double Fw_aero =( Lift * cos( Alpha ) - Drag * sin( Alpha ) );
	Fw_aero = -Fw_aero; // same Z convention

	double nz = Fw_aero /( MASS * G );
 // Level flight should read ≈ +1.0
 // If it reads ≈ -1.0, change to: Aero_D->G_meter =( float )( -nz );
	Aero_D->G_meter =( float )nz;


 //------------------------------------------------------------------
 // Outputs
 //------------------------------------------------------------------
	Aero_D->Pitch_d =( float )Pitch_d;
	Aero_D->Roll_d =( float )Roll_d;
	Aero_D->Yaw_d =( float )Yaw_d;

	if( strcmp( UNITS, "Imperial" ) == 0 ) 
	{
#if FIX_ALTITUDE_CONSISTENT
	Altitude_ft = -position.z;
#else
	Altitude_ft += Vw * dt;
#endif
	if( Altitude_ft < 0.0 ) Altitude_ft = 0.0;
	if( Altitude_ft > 50000.0 ) Altitude_ft = 50000.0;

	Aero_D->Altitude_AGL_feet =( float )Altitude_ft;
	Aero_D->RateOfClimb_ft_sec =( float )( Vw * 60.0 );
	Aero_D->AirSpeed_knots =( float )( Vu * 0.592 );
	}
}


//==============================================================================
// EOF








