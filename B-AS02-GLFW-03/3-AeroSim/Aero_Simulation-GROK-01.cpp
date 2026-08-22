//==============================================================================
//
// SIMULATION Function, Flightmodel-810-03  (cleaned drop-in)
//
//==============================================================================
#include "../2-MODEL/aero_model.h"
#include "../2-MODEL/coeff_table.h"
#include "Aero_Simulation.h"
#include "../structures.h"
#include "../IPC_UDP_Common.h"

void Aero_Simulation( float dt, Aerodyn_t* Aero_D );

//==============================================================================
// Constants
//==============================================================================
#define PI              3.14159265358979323846
#define DEGtoRAD        (PI / 180.0)
#define RADtoDEG        (180.0 / PI)
#define Sign(x)         (((x) < 0.0) ? (-1.0) : (1.0))

// Navion Aircraft Parameters (Imperial)
#define MAX_THRUST      360.0       // lbf
#define THRTL_INIT      0.7
#define ELV_TRIM        0.0
#define ELV_MAX_ANG     20.0        // deg
#define AIL_TRIM        0.0
#define AIL_MAX_ANG     30.0        // deg
#define RUD_TRIM        0.0
#define RUD_MAX_ANG     20.0        // deg
#define ALT_INIT        5000.0      // ft
#define V_CRUISE        210.0       // ft/s  (~143 kt)

#define G               32.174      // ft/s^2
#define RHO_0           0.002377    // slug/ft^3  sea level
#define RHO_10000       0.001756    // slug/ft^3  10 000 ft

#define _Ixx            948.0
#define _Iyy            1346.0
#define _Izz            1967.0
#define WEIGHT          2750.0      // lbf
#define MASS            (WEIGHT / G)
#define _C              4.9         // ft  mean aerodynamic chord
#define _S              184.0       // ft^2
#define _B              33.4        // ft  wing span

// Aerodynamic coefficients (linear model)
#define CL_0            0.270
#define CL_ALPHA        4.44        // /rad
#define CL_DELTA_E      0.335       // /rad   (not used in force eq here)
#define CD_0            0.025
#define K_IND           0.061       // induced drag factor

#define CM_0            0.0
#define CM_ALPHA       -0.683       // /rad
#define CM_Q           -0.7         // /rad  (will be non-dimensionalized)
#define CM_DELTA_E     -0.923       // /rad

#define Cl_0            0.0
#define Cl_DA          -0.134       // /rad
#define Cl_P           -0.410
#define Cl_R            0.107

#define CN_0            0.0
#define CN_b            0.0907      // /rad
#define CN_p           -0.0649
#define CN_r           -0.1199
#define CN_dr          -0.0805      // /rad
#define CN_da          -0.0504      // /rad  (available but not used)

#define CY_B           -0.404       // /rad
#define CY_DELTA_R      0.185       // /rad
#define CY_p           -0.145
#define CY_r            0.267

//==============================================================================
// Local types (match your existing headers as closely as possible)
//==============================================================================
typedef struct { double x, y, z; } Vec3;
typedef struct { double w, x, y, z; } Qtrn4f;

//==============================================================================
// Quaternion helpers (previously under #if 0)
//==============================================================================
static Qtrn4f quat_from_ang_rates(Vec3 W, double dt)
{
    double mag = sqrt(W.x*W.x + W.y*W.y + W.z*W.z);
    if (mag < 1.0e-12) {
        Qtrn4f q = {1.0, 0.0, 0.0, 0.0};
        return q;
    }
    double half_angle = 0.5 * mag * dt;
    double s = sin(half_angle);
    double c = cos(half_angle);
    Qtrn4f q = {
        c,
        s * W.x / mag,
        s * W.y / mag,
        s * W.z / mag
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
        q->w /= mag;  q->x /= mag;  q->y /= mag;  q->z /= mag;
    }
}

static Vec3 rotate_body_to_inrtl(Vec3 body, Qtrn4f q)
{
    // p = (0, body)
    Qtrn4f p  = {0.0, body.x, body.y, body.z};
    Qtrn4f qc = {q.w, -q.x, -q.y, -q.z};          // conjugate
    Qtrn4f t  = quat_multiply(q, p);
    Qtrn4f r  = quat_multiply(t, qc);
    Vec3 out = {r.x, r.y, r.z};
    return out;
}

static void Qtrn_to_Euler_deg(Qtrn4f q, double *Roll_d, double *Pitch_d, double *Yaw_d)
{
    // Roll (x)
    double sinr_cosp = 2.0 * (q.w*q.x + q.y*q.z);
    double cosr_cosp = 1.0 - 2.0 * (q.x*q.x + q.y*q.y);
    double roll = atan2(sinr_cosp, cosr_cosp);

    // Pitch (y)
    double sinp = 2.0 * (q.w*q.y - q.z*q.x);
    double pitch;
    if (fabs(sinp) >= 1.0)
        pitch = copysign(PI/2.0, sinp);
    else
        pitch = asin(sinp);

    // Yaw (z)
    double siny_cosp = 2.0 * (q.w*q.z + q.x*q.y);
    double cosy_cosp = 1.0 - 2.0 * (q.y*q.y + q.z*q.z);
    double yaw = atan2(siny_cosp, cosy_cosp);

    *Roll_d  = roll  * RADtoDEG;
    *Pitch_d = pitch * RADtoDEG;
    *Yaw_d   = yaw   * RADtoDEG;
    if (*Yaw_d < 0.0) *Yaw_d += 360.0;
}

//==============================================================================
// Simple density model (linear between SL and 10 kft)
//==============================================================================
static double atmosphere_density(double alt_ft)
{
    if (alt_ft < 0.0)     alt_ft = 0.0;
    if (alt_ft > 20000.0) alt_ft = 20000.0;
    // linear interpolation SL -> 10 kft, then hold
    double t = alt_ft / 10000.0;
    if (t > 1.0) t = 1.0;
    return RHO_0 + (RHO_10000 - RHO_0) * t;
}

//==============================================================================
// Main computation
//==============================================================================
void Aero_Computations(float dt, Aerodyn_t* Aero_D, Cntrls_t* CTRL)
{
    if (dt > 1.0f || dt <= 0.0f) return;

    //----------------------------------------------------------------------
    // Persistent state
    //----------------------------------------------------------------------
    static int    first_pass = 1;
    static double time = 0.0;

    static Vec3   W          = {0.0, 0.0, 0.0};   // body rates p,q,r
    static Qtrn4f _q         = {1.0, 0.0, 0.0, 0.0};
    static Vec3   position   = {0.0, 0.0, 0.0};   // inertial (ft)

    static double Vu = 0.0, Vv = 0.0, Vw = 0.0;   // body velocity
    static double Wp = 0.0, Wq = 0.0, Wr = 0.0;   // body rates
    static double Roll = 0.0, Pitch = 0.0, Yaw = 0.0; // rad
    static double Roll_d = 0.0, Pitch_d = 0.0, Yaw_d = 0.0;
    static double Altitude_ft = ALT_INIT;

    //----------------------------------------------------------------------
    // Initialization
    //----------------------------------------------------------------------
    if (first_pass) {
        first_pass = 0;
        Aero_D->Units = FPS;          // keep your existing enum

        time          = 0.0;
        Vu            = V_CRUISE;
        Vv            = 0.0;
        Vw            = 0.0;
        Wp = Wq = Wr  = 0.0;
        Altitude_ft   = ALT_INIT;
        position.x = position.y = 0.0;
        position.z = -ALT_INIT;       // Z positive up for this convention

        _q.w = 1.0; _q.x = _q.y = _q.z = 0.0;
        CTRL->Throttle_Cmd = THRTL_INIT;

        printf("Aero_Computations: Units set to FPS, initial speed %.1f ft/s @ %.0f ft\n",
               V_CRUISE, ALT_INIT);
    }

    time += dt;

    //----------------------------------------------------------------------
    // Controls  (keep original scaling, including the /5 on aileron)
    //----------------------------------------------------------------------
    double delta_e_deg = -(CTRL->Elevator_Cmd * ELV_MAX_ANG);
    double delta_e     = (delta_e_deg + ELV_TRIM) * DEGtoRAD;

    double delta_a_deg = (CTRL->Aileron_Cmd * AIL_MAX_ANG) / 5.0;  // original factor retained
    double delta_a     = delta_a_deg * DEGtoRAD;

    double delta_r_deg = (CTRL->Rudder_Cmd * RUD_MAX_ANG);
    double delta_r     = delta_r_deg * DEGtoRAD;

    double Thrust = CTRL->Throttle_Cmd * MAX_THRUST;

    //----------------------------------------------------------------------
    // Airspeed, Alpha, Beta
    //----------------------------------------------------------------------
    double Vinf_Sq = Vu*Vu + Vv*Vv + Vw*Vw;
    double Vinf    = sqrt(Vinf_Sq);
    if (Vinf < 1.0) Vinf = 1.0;          // prevent divide-by-zero

    double Alpha = atan2(Vw, Vu);                    // rad
    double Beta  = atan2(Vv, sqrt(Vu*Vu + Vw*Vw));   // more standard sideslip

    Aero_D->Alpha = Alpha * RADtoDEG;
    Aero_D->Beta  = Beta  * RADtoDEG;

    //----------------------------------------------------------------------
    // Dynamic pressure (altitude-dependent density)
    //----------------------------------------------------------------------
    double rho = atmosphere_density(Altitude_ft);
    double qS  = 0.5 * rho * Vinf_Sq * _S;
    double qSc = qS * _C;
    double qSb = qS * _B;

    //----------------------------------------------------------------------
    // Moments (body axes)
    //----------------------------------------------------------------------
    // Roll
    double Lm = qSb * ( Cl_0
                      + Cl_DA * delta_a
                      + Cl_P  * (Wp * _B / (2.0 * Vinf))
                      + Cl_R  * (Wr * _B / (2.0 * Vinf)) );
    double Ap = Lm / _Ixx;
    Wp += Ap * dt;

    // Pitch  – Cmq now non-dimensionalized
    double Mm = qSc * ( CM_0
                      + CM_ALPHA   * Alpha
                      + CM_Q       * (Wq * _C / (2.0 * Vinf))
                      + CM_DELTA_E * delta_e );
    double Aq = Mm / _Iyy;
    Wq += Aq * dt;

    // Yaw  – parentheses fixed
    double Nm = qSb * ( CN_0
                      + CN_b  * Beta
                      + CN_p  * (Wp * _B / (2.0 * Vinf))
                      + CN_r  * (Wr * _B / (2.0 * Vinf))
                      + CN_dr * delta_r );
    double Ar = Nm / _Izz;
    Wr += Ar * dt;

    W.x = Wp;  W.y = Wq;  W.z = Wr;

    //----------------------------------------------------------------------
    // Forces (body axes)
    //----------------------------------------------------------------------
    double CL = CL_0 + CL_ALPHA * Alpha;
    double Cd = CD_0 + K_IND * CL * CL;
    double Lift = qS * CL;
    double Drag = qS * Cd;

    // X (forward)
    double Fu = (Lift * sin(Alpha) - Drag * cos(Alpha)) + Thrust
              - WEIGHT * sin(Pitch);
    double Au = Fu / MASS;
    Vu += Au * dt;

    // Y (right)
    double Cy = CY_B * Beta
              + CY_DELTA_R * delta_r
              + CY_p * (Wp * _B / (2.0 * Vinf))
              + CY_r * (Wr * _B / (2.0 * Vinf));
    double Fv = qS * Cy;
    // Coriolis + gravity component
    double Av = Fv / MASS + Wr * Vu - Wp * Vw
              + G * cos(Pitch) * sin(Roll);
    Vv += Av * dt;

    // Z (down positive in many aero conventions – we keep the original flip)
    double Fw = (Lift * cos(Alpha) + Drag * sin(Alpha))
              - WEIGHT * cos(Roll) * cos(Pitch);
    Fw = -Fw;                         // original sign flip retained
    double Aw = Fw / MASS;
    Vw += Aw * dt;

    // Approximate load factor (normal)
    Aero_D->G_meter = -Fw / (MASS * G);

    //----------------------------------------------------------------------
    // Orientation update (quaternion)
    //----------------------------------------------------------------------
    Qtrn4f d_q = quat_from_ang_rates(W, dt);
    _q = quat_multiply(_q, d_q);
    normalize_quat(&_q);

    Qtrn_to_Euler_deg(_q, &Roll_d, &Pitch_d, &Yaw_d);
    Pitch = Pitch_d * DEGtoRAD;
    Roll  = Roll_d  * DEGtoRAD;
    Yaw   = Yaw_d   * DEGtoRAD;

    // Expose quaternion for display
    Qtrn_Orient_Disp.w = (float)_q.w;
    Qtrn_Orient_Disp.x = (float)_q.x;
    Qtrn_Orient_Disp.y = (float)_q.y;
    Qtrn_Orient_Disp.z = (float)_q.z;

    //----------------------------------------------------------------------
    // Inertial position (full body velocity rotated)
    //----------------------------------------------------------------------
    Vec3 V_body = {Vu, Vv, Vw};
    Vec3 Vi     = rotate_body_to_inrtl(V_body, _q);

    position.x += Vi.x * dt;
    position.y += Vi.y * dt;
    position.z += Vi.z * dt;          // Z positive up

    // Altitude from inertial Z (consistent)
    Altitude_ft = -position.z;        // because we initialized position.z = -ALT_INIT
    if (Altitude_ft < 0.0)     Altitude_ft = 0.0;
    if (Altitude_ft > 50000.0) Altitude_ft = 50000.0;

    //----------------------------------------------------------------------
    // Outputs
    //----------------------------------------------------------------------
    Aero_D->Position_inertial_X_ft = position.x;
    Aero_D->Position_inertial_Y_ft = position.y;
    Aero_D->Position_inertial_Z_ft = position.z;

    Aero_D->Pitch_d = Pitch_d;
    Aero_D->Roll_d  = Roll_d;
    Aero_D->Yaw_d   = Yaw_d;

    Aero_D->Altitude_AGL_feet   = Altitude_ft;
    Aero_D->RateOfClimb_ft_sec  = -Vw * 60.0;     // rough, body vertical
    Aero_D->AirSpeed_knots      = Vinf * 0.592483; // ft/s → knots

    // (optional TM block left disabled)
}

//==============================================================================
// Leftover stubs (kept for link compatibility – safe no-ops)
//==============================================================================
Vector3 getAttitude(void)
{
    Vector3 r = {0.0f, 0.0f, 0.0f};
    return r;
}

Vector3 getPosition(void)
{
    Vector3 r = {0.0f, 0.0f, 0.0f};
    return r;
}

//==============================================================================
// EOF
//==============================================================================
