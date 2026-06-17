##==============================================================================
##
##       SIMULATION Function,  Flightmodel-810-03
##
##==============================================================================

import math
import NavionAircraftParameters as airMdl

PI = math.pi
RADtoDEG = 180/PI

class Vec3:
    def __init__(self, x=0,y=0,z=0):
        self.x=x
        self.y=y
        self.z=z        

class Qtrn4:
    def __init__(self, w=0,x=0,y=0,z=0):
        self.w=w
        self.x=x
        self.y=y
        self.z=z        

def Sign( x ):
    if  x  < 0 :
        return -1
    return 1

##=======================================================
##  V           Linear Velocity x,y,z
##  A           Linear Acceleration  x,y,z
##
##  W           Angular Velocity x,y,z
##  W_dot       Angular Acceleration x,y,z
##  
##  F           Force x,y,z
##  T           Torgue x,y,z
##========================================================

class AeroModel():
    def __init__(self, dt, altInit, speed_fps, weight_lbs, units):
        self.params = airMdl()
        self.dt = dt
        self.time = 0.0

##===========================================================================================================
## Quaternions
        self._q = Qtrn4f(1.0, 0.0, 0.0, 0.0) #< initial orientation    
        self.W = Vec3()
        self.Vinf_V = Vec3() 
        self.position = Vec3() 
        self.V = Vec3()  
    
##===========================================================================================================
        self.Roll_d = 0.0  #< Y (+) right
        self.Pitch_d = 0.0 #< P (+) up
        self.Yaw_d = 0.0   #< R (+) right
        self.Roll = 0.0    #< Radians
        self.Pitch = 0.0   #< Radians
        self.Yaw = 0.0     #< Radians       

        self.Fu, self.Fv, self.Fw = (0.0, 0.0, 0.0) #< X, Y, Z      linear Force (forward, right, down )
        self.Au, self.Av, self.Aw = (0.0, 0.0, 0.0) #< u_dot, v_dot, w_dot linear accel     
        self.Vu, self.Vv, self.Vw = (speed_fps;, 0.0, 0.0) #< u, v, w  linear Velocity 
        self.Px, self.Py, self.Pz = (0.0, 0.0, 0.0) #< X, Y, Z  linear Position 

        self.Lm, self.Mm, self.Nm = (0.0, 0.0, 0.0) #< L, M, N (xyz) angular moment
        self.Ap, self.Aq, self.Ar = (0.0, 0.0, 0.0) #< p_dot, q_dot, r_dot (xyz) angular accel
        self.Wp, self.Wq, self.Wr = (0.0, 0.0, 0.0) #< p, q, r (xyz) angular rate
        self.Pp, self.Pq, self.Pr = (0.0, 0.0, 0.0) #< Roll (x), Pitch (y), Yaw (z) angular position

        self.Vinf = 0.0;           #< ft/sec
        self.Vinf_Sq              = 0.0;

        self.Alpha, self.Alpha2 = (0.0, 0.0)  #< angle of attack 
        self.Beta = 0.0           #< sideslip angle 
        self.Lift, self.Drag = (0.0, 0.0)
        self.Altitude_ft = altInit_ft
        self.Altitude_m  = altInit_ft / 3.28084F;          ## 1 ft = 3.28094 meters

        self.Wind = Vec3()           #< wind vector [m/s] 

        self.Weight = weight_lbs;    ## Weight lbs : mass (lbs/G slugs )
        self.Thrust = 0.0 

        self.delta_e_deg = 0.0    #< Elevator deflection (degrees)
        self.delta_e = 0.0        #< Elevator deflection (radians)
        self.delta_e_trim = 0.0

        self.delta_a_deg = 0.0
        sself.delta_a = 0.0

        self.delta_r_deg = 0.0
        self.delta_r = 0.0

        if (units == "Metric") or (units == "Imperial"):    
            print("UNITS set to %s\n"%units);
        else:
            print(">>>>> Units Not defined %s\n"%units);  
       

    def step(self, Cntrls, dt=None ):
        dt = self.dt if dt==None else dy
        self.time += dt
        
        ##================ Controls ================================================================================================
        self.delta_e_deg  = -(Cntrls.Elevator_Cmd * self.params.ELV_MAX_ANG )      #< Pitch stick y axis range -1.0 to 1.0
        self.delta_e_trim = self.params.ELV_TRIM
        self.delta_e      = (self.delta_e_deg + self.delta_e_trim) * DEGtoRAD

        self.delta_a_deg  = (Cntrls.Aileron_Cmd * self.params.AIL_MAX_ANG ) / 5.0  #< Roll stick x axis range -1.0 to 1.0
        self.delta_a      = self.delta_a_deg * DEGtoRAD

        self.delta_r_deg  = (Cntrls.Rudder_Cmd * self.params.RUD_MAX_ANG )         #< Roll stick x axis range -1.0 to 1.0
        self.delta_r      = self.delta_r_deg * DEGtoRAD

        self.Thrust = Cntrls.Throttle_Cmd * self.params.MAX_THRUST                 #< Throttle Command setting [ 0, 1]
        ##================== End Controls ============================================================================================

        ##================== Airspeed, Alpha, Beta, Flight Path ======================================================================
        ## Update the airspeed 
        if abs(self.Vu) < 0.001:       #< prevent devide by zero
            self.Alpha = 0.0
            self.Beta  = 0.0
        else:
            self.Alpha = math.atan2(  self.Vw, self.Vu )   #< geometry - rise over run 
            self.Beta  = math.atan2( -self.Vv, self.Vu )

        self.Vinf    = math.sqrt( self.Vu**2 +self.Vw**2);   
        self.Vinf_Sq = self.Vu**2 +self.Vv**2 +self.Vw**2
        self.Vinf    = math.sqrt(self.Vinf_Sq)

        Aero.Alpha = self.Alpha * RADtoDEG
        Aero.Beta  = self.Beta * RADtoDEG

        sideLift = 0.0
        sideDrag = 0.0

        qS  = 0.5 * self.params.RHO * (self.params.Vinf**2) * self.params._S
        qSc = qS * self.params._C
        qSb = qS * self.params._B
        ##============================================================================================================================
        
        Cmo   = self.parames.CM_0
        Cmq   = self.parames.CM_Q 
        Cmde  = self.parames.CM_DELTA_E
        Cma   = self.parames.CM_ALPHA
        
        Clo   = self.parames.Cl_0     #< Roll, Zero-control moment ( typically small or zero in symmetric flight )
        Clda  = self.parames.Cl_DA    #< Roll, Aileron effectiveness 9 change in Cl per radian of aileron deflection )
        Clp   = self.parames.Cl_P     #< Roll, Damping ( change in Cl per unit of roll rate ) 
        Clr   = self.parames.Cl_R     #< Roll, Yaw-roll coupling ( change in Cl per unti of yaw rate. )
        
        Cno   = 0.0;
        Cnb   = self.parames.CN_b     #< Yaw, sideslip moment, yaw stability
        Cnp   = self.parames.CN_p     #< Yaw, roll-rate moment, rikk-yaw coupling 
        Cnr   = self.parames.CN_r     #< Yaw, yaw-rate moment, yaw damping
        Cndr  = self.parames.CN_dr    #< Yaw, rudder deflection moment, rudder effectiveness
        Cnda  = self.parames.CN_da    #< Yaw, aileron defection moment, aileron inducted yaw  

        ##=======================================================================================================================
        ## Momenets and rotation
        ## X axis
        Lm = qSb * ( Clo +( Clda * self.delta_a ) +( Clp*self.Wp*_B/(2*self.Vinf)) +( Clr*self.Wr*_B/(2*self.Vinf)))
        Ap = Lm / self.parames._Ixx ## calc roll rate radians/sec. (Force / Moment_Inertia) * time 
        self.Wp += Ap * dt
        
        ## Y axis
        Mm = qSc * ( Cmde * self.delta_e +Cma * self.Alpha +Cmq * self.Wq )
        Aq = Mm / self.parames._Iyy ## calc pitch rate radians/sec. (Force / Moment_Inertia) * time     
        self.Wq += Aq * dt

        ## Z axis 
        Nm = qSb * ( Cno +(Cnb * self.Beta) + (Cnp * self.Wp*self.parames._B/(2*self.Vinf)) +( Cnr * self.Wr*self.parames._B/(2*Vinf) +( Cndr * self.delta_r ) ))
        Ar = Nm / self.parames._Izz      ## calc yaw rate radians/sec. (Force / Moment_Inertia) * time
        self.Wr += Ar * dt
        ##=======================================================================================================================   

        ## Lift and drag forces - linear
        CLo   = self.parames.CL_0
        CLa   = self.parames.CL_ALPHA     
        CDo   = self.parames.CD_0
        Cyb   = self.parames.CY_B
        Cydr  = self.parames.CY_DELTA_R
        Cyp   = self.parames.CY_p
        Cyr   = self.parames. CY_r

        CL = ( CLo + ( CLa * self.Alpha ))
        Cd = ( CDo + ( self.parames.K*(CL**2) ))
        
        self.Lift = qS * CL
        self.Drag = qS * Cd
        ##===============================================================   

        ## X Axis
        self.Fu = self.Lift * math.sin(self.Alpha) -self.Drag * math.cos(self.Alpha) +self.Thrust -self.Weight * math.sin(self.Pitch)
        self.Au = self.Fu / self.parames.MASS  
        self.Vu += self.Au * dt

        ## Y Axis
        Cy = Cyb * self.Beta\
             +Cydr * self.delta_r\
             +Cyp * ( self.Wp * self.Vw / (2*self.Vinf ))\
             +Cyr * ( self.Wr * self.Vw / (2*self.Vinf ))

        self.Fv = qS * Cy
        self.Av = (self.Fv / self.params.MASS ) +self.Wr * self.Vu -self.Wp * self.Vw +self.parames.G * math.cos(self.Pitch) * math.sin(self.Roll)
        self.Vv += self.Av * dt
    
        ## Z axis, (-) to flip for Z axis sign convention, right hand rule
        self.Fw = -((( self.Lift * math.cos( self.Alpha )) -( self.Drag * math.sin( self.Alpha )) -( self.Weight * math.cos( self.Roll ) * math.cos( self.Pitch ))))
        self.Aw = self.Fw / self.parames.MASS
        self.Vw += self.Aw * dt
        ##=======================================================================================================================   

        ## Body to Intertial transform
        d_q = Qtrn4()
        Vi_inertial = Vec3()

        self.W.x = self.Wp
        self.W.y = self.Wq
        self.W.z = self.Wr
        
        ## Quaternion process, next 5 lines
        d_q = quat_from_ang_rates( self.W, dt )   
        self._q  = quat_multiply( self._q, d_q )
        normalize_quat( self._q )
        Vi_inertial = rotate_body_to_inrtl( self.Vinf_V, self._q ) ##< ???       
        Qtrn_to_Euler_deg( self._q, self.Roll_d, self.Pitch_d, self.Yaw_d )

        Vinf_V.x = self.Vu
        Vinf_V.y = self.Vv

        ## 6DOF inertial solution       
        self.position.x += Vi_inertial.x * dt
        self.position.y += Vi_inertial.y * dt
        self.position.z -= Vi_inertial.z * dt
        
        self.Pitch = self.Pitch_d * DEGtoRAD
        self.Roll = self.Roll_d * DEGtoRAD
        self.Yaw  = self.Yaw_d * DEGtoRAD 
        ##=================================================================================================================


##========================================================================================================================
##  Retained needed C++ code

##Vector3 FlightModel::getAttitude( )
Vector3 getAttitude( )
{
 ## Vector3 result( b_Roll, b_Pitch, b_Yaw );

  Vector3 result;
    result.x = b_Roll;
    result.y = b_Pitch;
    result.z = b_Yaw;
    return result;
}

##Vector3 FlightModel::getPosition( )
Vector3 getPosition( )
{
##Vector3 result( b_X, b_Y, b_Z );
## Vector3 result( 1.0, 2.0, 3.0 );

  Vector3 result;
    result.x = b_X;
    result.y = b_Y;
    result.z = b_Z;
    return result;
}

##=================================================================================================================
## ChatGTP Quaternion Code

## Rotate body Vel to inrt frame
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

## Compute Heading Ang_le ( _radians )
float Heading( const Qtrn4f &q ) 
{
     float x = q.x, y = q.y, z = q.z, w = q.w;

     ## Rotate body x-axis ( 1,0,0 ) Int_o inrt frame
     float fx = 1 - 2 * ( y*y + z*z );
     float fz = 2 * ( x*z - w*y );
     float Heading_rad = std::atan2( fx, fz ); ## atan2( east, north )
     return Heading_rad;
}

## Create delta quaternion from angular Vi
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
 
## Quaternion multiplication
Qtrn4f quat_multiply( Qtrn4f q1, Qtrn4f q2 ) 
{ 
    Qtrn4f r;
    r.w = q1.w * q2.w - q1.x * q2.x - q1.y * q2.y - q1.z * q2.z;
    r.x = q1.w * q2.x + q1.x * q2.w + q1.y * q2.z - q1.z * q2.y;
    r.y = q1.w * q2.y - q1.x * q2.z + q1.y * q2.w + q1.z * q2.x;
    r.z = q1.w * q2.z + q1.x * q2.y - q1.y * q2.x + q1.z * q2.w;
    return r;
}

## Normalize quaternion
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

## Rotate body vector to inertial frame using quaternion
Vec3 rotate_body_to_inrtl( Vec3 body, Qtrn4f q ) 
{ 
    Qtrn4f p = { 0, body.x, body.y, body.z };
    Qtrn4f q_conj = { q.w, -q.x, -q.y, -q.z };
    Qtrn4f rotated = quat_multiply( quat_multiply( q, p ), q_conj );
    return( Vec3 ){ rotated.x, rotated.y, rotated.z };
}

## Extract Euler angles from quaternion( roll, pitch, yaw in degrees )
void Qtrn_to_Euler_deg( Qtrn4f q, double *Roll_d, double *Pitch_d, double *Yaw_d ) 
{ 
    ## Roll( X-axis rotation )
    double sinr_cosp = 2.0 * ( q.w * q.x + q.y * q.z );
    double cosr_cosp = 1.0 - 2.0 * ( q.x * q.x + q.y * q.y );
    double roll = atan2( sinr_cosp, cosr_cosp );

    ## Pitch( Y-axis rotation )
    double sinp = 2.0 * ( q.w * q.y - q.z * q.x );
    double pitch;
    if( fabs( sinp ) >= 1.0 )
        pitch = copysign( M_PI / 2.0, sinp );
    else
        pitch = asin( sinp );

    ## Yaw( Z-axis rotation )
    double siny_cosp = 2.0 * ( q.w * q.z + q.x * q.y );
    double cosy_cosp = 1.0 - 2.0 * ( q.y * q.y + q.z * q.z );
    double yaw = atan2( siny_cosp, cosy_cosp );

    ## Convert to Degrees
    *Roll_d     = roll * 180.0 / M_PI;
    
    *Pitch_d    = pitch * 180.0 / M_PI;
    
    *Yaw_d      = yaw * 180.0 / M_PI;
    if( *Yaw_d < 0.0 ) * Yaw_d += 360.0;
 }
 
## End - ChatGTP Quaternion Code
##================================================================================================================= 
