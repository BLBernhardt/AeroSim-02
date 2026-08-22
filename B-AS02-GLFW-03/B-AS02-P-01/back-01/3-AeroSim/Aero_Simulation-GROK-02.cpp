//==============================================================================
//
// SIMULATION Function, Flightmodel-810-03
// Starting point = original code + incremental switches
//
//==============================================================================
#include "../2-MODEL/aero_model.h"
#include "../2-MODEL/coeff_table.h"
#include "Aero_Simulation.h"
#include "../structures.h"
#include "../IPC_UDP_Common.h"

void Aero_Simulation( float dt, Aerodyn_t* Aero_D );

//==============================================================================
// SWITCHES – start with all at 0 except FIX_QUAT_HELPERS
// Turn them on one at a time after confirming stability.
//==============================================================================
#define FIX_QUAT_HELPERS        1   // required for the orientation path to exist
#define FIX_FULL_BODY_VEL       1   // complete body velocity before rotation
#define FIX_YAW_PARENTHESES     1   // correct parentheses in yaw moment
#define FIX_ALTITUDE_CONSISTENT 1   // derive altitude from inertial position
#define FIX_DENSITY_ALT         1   // altitude-dependent density
#define FIX_ALPHA_BETA          0   // alternative Beta definition
#define FIX_CMQ_NONDIM          0   // non-dimensionalize Cmq (changes damping a lot)

//==============================================================================
// Constants (original values)
//==============================================================================


#define MAX_THRUST 360.0
#define THRTL_INIT 0.7
#define ELV_TRIM 0.0
#define ELV_MAX_ANG 20.0
#define AIL_TRIM 0.0
#define AIL_MAX_ANG 30.0
#define RUD_TRIM 0.0
#define RUD_MAX_ANG 20.0
#define ALT_INIT 5000.0
#define V_CRUISE 210.0

#define UNITS "Imperial"
#define G 32.1740
#define RHO_0 0.002377
#define RHO_10000 0.001756
#define RHO RHO_10000

#define _Iyy 1346.0
#define _Ixx 948.0
#define _Izz 1967.0
#define WEIGHT 2750.0
#define MASS (WEIGHT/G)
#define _C 4.9
#define _S 184.0
#define _B 33.4

#define CL_0 0.270
#define CL_ALPHA 4.44
#define CL_DELTA_E 0.335
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
#define CN_0 -0.0
#define CN_b 0.0907
#define CN_p -0.0649
#define CN_r -0.1199
#define CN_dr -0.0805
#define CN_da -0.0504
#define CY_B -0.404
#define CY_DELTA_R 0.185
#define CY_r 0.267
#define CY_p -0.145




//==============================================================================
// Local types (matching original usage)
//==============================================================================
//typedef struct { double x, y, z; } Vec3;
//typedef struct { double w, x, y, z; } Qtrn4f;

static Qtrn4f quat_from_ang_rates(Vec3 W, double dt);
static Qtrn4f quat_multiply(Qtrn4f q1, Qtrn4f q2);
static void normalize_quat(Qtrn4f *q);
static Vec3 rotate_body_to_inrtl(Vec3 body, Qtrn4f q);
static void Qtrn_to_Euler_deg(Qtrn4f q, double *Roll_d, double *Pitch_d, double *Yaw_d);

//==============================================================================
// Density helper
//==============================================================================
static double get_rho(double alt_ft)
{
#if FIX_DENSITY_ALT
    if (alt_ft < 0.0) alt_ft = 0.0;
    if (alt_ft > 20000.0) alt_ft = 20000.0;
    double t = alt_ft / 10000.0;
    if (t > 1.0) t = 1.0;
    return RHO_0 + (RHO_10000 - RHO_0) * t;
#else
    return RHO;          // original hard-coded value
#endif
}

//==============================================================================
void Aero_Computations( float dt, Aerodyn_t* Aero_D, Cntrls_t* CTRL )
{
    if( dt > 1.0f ) return;

    static double time;

    static Vec3 W = { 0.0, 0.0, 0.0 };
    static Qtrn4f _q = { 1, 0, 0, 0 };
    static Vec3 Vinf_V = { 0.0, 0.0, 0.0 };
    static Vec3 position = { 0.0, 0.0, 0.0 };
    static Quaternion4 OrientationQuat;

    static double Roll_d, Pitch_d, Yaw_d;
    static double Roll, Pitch, Yaw;

    static double Fu, Fv, Fw;
    static double Au, Av, Aw;
    static double Vu, Vv, Vw;
    static double Lm, Mm, Nm;
    static double Ap, Aq, Ar;
    static double Wp, Wq, Wr;
    static double Vinf;
    static double Alpha;
    static double Beta;
    static double Lift, Drag;
    static double Altitude_ft = ALT_INIT;
    static double Weight, Thrust;
    static double delta_e_deg, delta_e, delta_e_trim;
    static double delta_a_deg, delta_a;
    static double delta_r_deg, delta_r;
    float qS, qSc, qSb;
    double Vinf_Sq;

    static int first_pass = 1;

    //================== Init ==================================================
    if( first_pass )
    {
        first_pass = 0;
        Aero_D->Units = FPS;

        if( Aero_D->Units == METRIC )
            printf("1] UNITS set to Metric \n");
        else if( Aero_D->Units == FPS )
            printf("2] UNITS set to FPS \n");
        else
            printf(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Units Not defined \n <<<<<<<<<<<<<<<<<<<<<<<<< ");

        time = -1.0;

        Weight = WEIGHT;
        OrientationQuat = Quaternion_RotationYawPitchRoll( 0.0f, 0.0f, 0.0f );
        CTRL->Throttle_Cmd = THRTL_INIT;
        Vv = 0.0;
        Vw = 0.0;
        Vu = V_CRUISE;
        Altitude_ft = ALT_INIT;
        CTRL->Throttle_Cmd = THRTL_INIT;
        Vinf_Sq = 0.0;
    }
    //================== End Init ==============================================

    time += dt;

    //================ Controls ================================================
    delta_e_deg = -(CTRL->Elevator_Cmd * ELV_MAX_ANG );
    delta_e_trim = ELV_TRIM;
    delta_e = (delta_e_deg + delta_e_trim) * DEGtoRAD;

    delta_a_deg = (CTRL->Aileron_Cmd * AIL_MAX_ANG ) / 5.0 ;
    delta_a = delta_a_deg * DEGtoRAD;

    delta_r_deg = (CTRL->Rudder_Cmd * RUD_MAX_ANG );
    delta_r = delta_r_deg * DEGtoRAD;

    Thrust = CTRL->Throttle_Cmd * MAX_THRUST;
    Weight = WEIGHT;

    //================== Airspeed, Alpha, Beta =================================
    if( abs( Vu ) < 0.001 )
    {
        Alpha = 0.0;
        Beta = 0.0;
    }
    else
    {
#if FIX_ALPHA_BETA
        Alpha = atan2( Vw, Vu );
        Beta  = atan2( Vv, sqrt(Vu*Vu + Vw*Vw) );
#else
        // original
        Alpha = atan2( Vw, Vu );
        Beta  = atan2( -Vv, Vu );
#endif
    }

    Vinf_Sq = Vu * Vu + Vv * Vv + Vw * Vw;
    Vinf = sqrt( Vinf_Sq );
    if (Vinf < 1.0) Vinf = 1.0;   // small safety

    Aero_D->Alpha = Alpha * RADtoDEG;
    Aero_D->Beta  = Beta  * RADtoDEG;

    double rho = get_rho(Altitude_ft);
    qS  = 0.5 * rho * Vinf * Vinf * _S;
    qSc = qS * _C;
    qSb = qS * _B;

    //================== Moments ===============================================
    float Cmq  = CM_Q;
    float Cmde = CM_DELTA_E;
    float Cma  = CM_ALPHA;

    float Clo  = Cl_0;
    float Clda = Cl_DA;
    float Clp  = Cl_P;
    float Clr  = Cl_R;

    float Cno  = 0.0;
    float Cnb  = CN_b;
    float Cnp  = CN_p;
    float Cnr  = CN_r;
    float Cndr = CN_dr;

    // X axis (roll)
    Lm = qSb * ( ( Clo ) + ( Clda * delta_a ) + ( Clp*Wp*_B/(2.0*Vinf)) + ( Clr*Wr*_B/(2.0*Vinf)));
    Ap = ( Lm / _Ixx );
    Wp += Ap * dt;

    // Y axis (pitch)
#if FIX_CMQ_NONDIM
    Mm = qSc * ( ( Cmde * delta_e ) + (Cma * Alpha) + ( Cmq * (Wq * _C / (2.0 * Vinf)) ) );
#else
    // original – raw rate
    Mm = qSc * ( ( Cmde * delta_e ) + (Cma * Alpha) + ( Cmq * Wq ) );
#endif
    Aq = ( Mm / _Iyy );
    Wq += Aq * dt;

    // Z axis (yaw)
#if FIX_YAW_PARENTHESES
    Nm = qSb * ( Cno + ( Cnb * Beta ) + ( Cnp * Wp*_B/(2.0*Vinf)) + ( Cnr * Wr*_B/(2.0*Vinf) ) + ( Cndr * delta_r ) );
#else
    // original (parentheses as written)
    Nm = qSb * ( Cno + ( Cnb * Beta ) + ( Cnp * Wp*_B/(2.0*Vinf)) + ( Cnr * Wr*_B/(2.0*Vinf) + ( Cndr * delta_r ) ));
#endif
    Ar = ( Nm / _Izz );
    Wr += Ar * dt;

    //================== Forces ================================================
    float CLo = CL_0;
    float CLa = CL_ALPHA;
    float CDo = CD_0;
    float CL, Cd, Cy;
    float Cyb = CY_B;
    float Cydr = CY_DELTA_R;
    float Cyp = CY_p;
    float Cyr = CY_r;

    CL = ( CLo + ( CLa * Alpha ));
    Cd = ( CDo + ( K*CL*CL ));
    Lift = qS * CL;
    Drag = qS * Cd;

    // X Axis
    Fu = ( float )( ( Lift * sin( Alpha )) - ( Drag * cos( Alpha )) + ( Thrust ) - ( Weight * sin( Pitch )));
    Au = ( float )( Fu / MASS );
    Vu += Au * dt;

    // Y Axis
    Cy = Cyb * Beta
       + Cydr * delta_r
       + Cyp * ( Wp * Vw / (2.0 * Vinf ))
       + Cyr * ( Wr * Vw / (2.0 * Vinf ));
    Fv = qS * Cy;
    Av = (Fv / MASS ) + Wr * Vu - Wp * Vw + G * cos(Pitch) * sin(Roll);
    Vv += Av * dt;

    // Z axis
    Fw = ( float )(( Lift * cos( Alpha )) - ( Drag * sin( Alpha )) - ( Weight * cos( Roll ) * cos( Pitch )));
    Fw = -Fw;
    Aw = ( float )( Fw / MASS );
    Vw += Aw * dt;

    W.x = Wp;
    W.y = Wq;
    W.z = Wr;

    if( Aero_D->Units == METRIC ) Aero_D->G_meter = Wq/32.2;
    if( Aero_D->Units == FPS )    Aero_D->G_meter = Wq/9.81;

    //================== Orientation + Position ================================
#if FIX_QUAT_HELPERS
    Qtrn4f d_q;
    Vec3 Vi_inertial;

    d_q = quat_from_ang_rates( W, dt );
    _q = quat_multiply( _q, d_q );
    normalize_quat( &_q );

#if FIX_FULL_BODY_VEL
    Vec3 V_body = { Vu, Vv, Vw };
    Vi_inertial = rotate_body_to_inrtl( V_body, _q );
#else
    // original (partial update)
    Vi_inertial = rotate_body_to_inrtl( Vinf_V, _q );
    Vinf_V.x = Vu;
    Vinf_V.y = Vv;
    // note: Vinf_V.z never written in original
#endif

    Qtrn_to_Euler_deg( _q, &Roll_d, &Pitch_d, &Yaw_d );

    position.x += Vi_inertial.x * dt;
    position.y += Vi_inertial.y * dt;
    position.z -= Vi_inertial.z * dt;

    Aero_D->Position_inertial_X_ft = position.x;
    Aero_D->Position_inertial_Y_ft = position.y;
    Aero_D->Position_inertial_Z_ft = position.z;

    Pitch = Pitch_d * DEGtoRAD;
    Roll  = Roll_d  * DEGtoRAD;
    Yaw   = Yaw_d   * DEGtoRAD;

    Qtrn_Orient_Disp.x = _q.x;
    Qtrn_Orient_Disp.y = _q.y;
    Qtrn_Orient_Disp.z = _q.z;
    Qtrn_Orient_Disp.w = _q.w;
#endif

    //================== PFD / Altitude ========================================
    Aero_D->Pitch_d = Pitch * RADtoDEG;
    Aero_D->Roll_d  = Roll  * RADtoDEG;
    Aero_D->Yaw_d   = Yaw   * RADtoDEG;

    if( strcmp( UNITS, "Imperial" ) == 0 )
    {
#if FIX_ALTITUDE_CONSISTENT
        // optional later: derive from inertial
        Altitude_ft = -position.z;   // only if you change the position.z sign convention
#else
        // original
        Altitude_ft += Vw * dt;
#endif
        Aero_D->RateOfClimb_ft_sec = Vw * 60.0 ;
        if( Altitude_ft > 50000.0 ) Altitude_ft = 50000.0 ;
        if( Altitude_ft < 0.0 )     Altitude_ft = 0.0 ;
        Aero_D->Altitude_AGL_feet = Altitude_ft;
        Aero_D->AirSpeed_knots = Vu * 0.592;
    }
    else
        printf( "12]>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Units Not defined \n <<<<<<<<<<<<<<<<<<<<<<<<< " );
}


//==============================================================================
// Quaternion helpers – only present when FIX_QUAT_HELPERS == 1
//==============================================================================
#if FIX_QUAT_HELPERS





static Qtrn4f quat_from_ang_rates(Vec3 W, double dt)
{
    double mag = sqrt(W.x*W.x + W.y*W.y + W.z*W.z);
    if (mag < 1.0e-12) {
        Qtrn4f q = {1.0, 0.0, 0.0, 0.0};
        return q;
    }
    double half_angle = 0.5 * mag * dt;
    double sin_half = sin(half_angle);
    double cos_half = cos(half_angle);
    Qtrn4f q = {
        cos_half,
        sin_half * W.x / mag,
        sin_half * W.y / mag,
        sin_half * W.z / mag
    };
    return q;
}

static Qtrn4f quat_multiply(Qtrn4f q1, Qtrn4f q2)
{
    Qtrn4f r;
    r.w = q1.w*q2.w - q1.x*q2.x - q1.y*q2.y - q1.z*q2.z;
    r.x = q1.w*q2.x + q1.x*q2.w + q1.y*q2.z - q1.z*q2.y;
    r.y = q1.w*q2.y - q1.x*q2.z + q1.y*q2.w + q1.z*q2.x;
    r.z = q1.w*q2.z + q1.x*q2.y - q1.y*q2.x + q1.z*q2.w;
    return r;
}

static void normalize_quat(Qtrn4f *q)
{
    double mag = sqrt(q->w*q->w + q->x*q->x + q->y*q->y + q->z*q->z);
    if (mag > 1.0e-12) {
        q->w /= mag;
        q->x /= mag;
        q->y /= mag;
        q->z /= mag;
    }
}

static Vec3 rotate_body_to_inrtl(Vec3 body, Qtrn4f q)
{
    Qtrn4f p = {0.0, body.x, body.y, body.z};
    Qtrn4f q_conj = {q.w, -q.x, -q.y, -q.z};
    Qtrn4f rotated = quat_multiply(quat_multiply(q, p), q_conj);
    Vec3 out = {rotated.x, rotated.y, rotated.z};
    return out;
}

static void Qtrn_to_Euler_deg(Qtrn4f q, double *Roll_d, double *Pitch_d, double *Yaw_d)
{
    double sinr_cosp = 2.0 * (q.w * q.x + q.y * q.z);
    double cosr_cosp = 1.0 - 2.0 * (q.x * q.x + q.y * q.y);
    double roll = atan2(sinr_cosp, cosr_cosp);

    double sinp = 2.0 * (q.w * q.y - q.z * q.x);
    double pitch;
    if (fabs(sinp) >= 1.0)
        pitch = copysign(PI / 2.0, sinp);
    else
        pitch = asin(sinp);

    double siny_cosp = 2.0 * (q.w * q.z + q.x * q.y);
    double cosy_cosp = 1.0 - 2.0 * (q.y * q.y + q.z * q.z);
    double yaw = atan2(siny_cosp, cosy_cosp);

    *Roll_d  = roll  * RADtoDEG;
    *Pitch_d = pitch * RADtoDEG;
    *Yaw_d   = yaw   * RADtoDEG;
    if (*Yaw_d < 0.0) *Yaw_d += 360.0;
}

#endif // FIX_QUAT_HELPERS





//==============================================================================
// Leftover stubs (kept for compatibility)
//==============================================================================
Vector3 getAttitude( )
{
    Vector3 result;
    result.x = 0.0; result.y = 0.0; result.z = 0.0;
    return result;
}

Vector3 getPosition( )
{
    Vector3 result;
    result.x = 0.0; result.y = 0.0; result.z = 0.0;
    return result;
}
//==============================================================================
