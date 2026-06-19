##==============================================================================
##
##       SIMULATION Function,  Flightmodel-810-03
##
##==============================================================================

import math
import NavionAircraftParameters as airMdl

PI = math.pi
RADtoDEG = 180/PI

def Sign( x ):
    if  x  < 0 :
        return -1
    return 1

class Vec_xyz():
    def __init__(self, x=0,y=0,z=0):
        self.x=x #< Forward
        self.y=y #< Right
        self.z=z #< Down

    def mag(self) -> float:
        return math.sqrt(self.x**2 +self.y**2 +self.z**2)

class Vec_pqr():
    def __init__(self, p=0,q=0,r=0):
        self.p=p #< Roll
        self.q=q #< Pitch
        self.r=r #< Yaw

    def mag(self) -> float:
        return math.sqrt(self.p**2 +self.q**2 +self.r**2)

    def getQuat(self, dt ):
        """  """
        mag = self.mag()
        if ( mag == 0.0 ):
            return Qtrn( 1, 0, 0, 0 )

        half_angle = 0.5 * mag * dt
        sin_half = math.sin( half_angle )
        cos_half = math.cos( half_angle )

        return Qtrn(
            w=cos_half,
            x=sin_half * self.p / mag,
            y=sin_half * self.q / mag,
            z=sin_half * self.r / mag)

class Qtrn():
    def __init__(self, w=0,x=0,y=0,z=0):
        self.w=w
        self.x=x
        self.y=y
        self.z=z        
     
    def mag(self) -> float:
        return math.sqrt(self.w**2, self.x**2 +self.y**2 +self.z**2)

    def multiply( self, q2 ): 
        """Quaternion multiplication"""
        q1 = self
        w = q1.w * q2.w - q1.x * q2.x - q1.y * q2.y - q1.z * q2.z
        x = q1.w * q2.x + q1.x * q2.w + q1.y * q2.z - q1.z * q2.y
        y = q1.w * q2.y - q1.x * q2.z + q1.y * q2.w + q1.z * q2.x
        z = q1.w * q2.z + q1.x * q2.y - q1.y * q2.x + q1.z * q2.w
        self.w, self.x, self.y, self.z  = (w,x,y,z)
        return self

    def normalize(self) -> None: 
        """Normalize quaternion"""
        mag = self.mag()
        if mag > 0.0:
            self.w /= mag
            self.x /= mag
            self.y /= mag
            self.z /= mag
        else:
            printf("Error - normalize_quat() - something wrong with quaternion - divide by zero, mag = 0.0 \n");
        return self

    def getEuler(self): 
        """Extract Euler angles from quaternion( roll, pitch, yaw in degrees )"""
        ## Roll( X-axis rotation )
        sinr_cosp = 2.0*( self.w * self.x + self.y * self.z )
        cosr_cosp = 1.0 -2.0*( self.x**2 +self.y**2 )
        roll = math.atan2( sinr_cosp, cosr_cosp )

        ## Pitch( Y-axis rotation )
        sinp = 2.0 * ( self.w * self.y - self.z * self.x )
        if( abs(sinp) >= 1.0 ):
            pitch = Sign(sinp)*M_PI / 2.0
        else:
            pitch = math.asin( sinp )

        ## Yaw( Z-axis rotation )
        siny_cosp = 2.0 * ( self.w * self.z + self.x * self.y )
        cosy_cosp = 1.0 - 2.0 * ( self.y**2 + q.z**2 )
        yaw = math.atan2( siny_cosp, cosy_cosp )
        return Vec_pqr(p=roll, q=pitch, r=yaw)

    def Heading(self): 
        """Compute Heading Ang_le ( radians )"""
        ## Rotate body x-axis ( 1,0,0 ) Int_o inrt frame
        fx = 1 - 2*(self.y**2 +self.z**2)
        fz = 2 * ( self.x*self.z - self.w*self.y )
        return math.atan2( fx, fz ) #< atan2( east, north )

class Attitude():
    def __init__(self, roll_r=0.0, pitch_r=0.0, yaw_r=0.0):
        self.roll_r  = roll_r
        self.pitch_r = pitch_r
        self.yaw_r   = yaw_r        

class AeroModel():
    def __init__(self, dt, altInit_m, speed_fps, weight_lbs, units):
        self.params = airMdl()
        self.dt = dt
        self.time = 0.0

##===========================================================================================================
## Quaternions
#        self.position = Vec3() 
        self.P = Vec_xyz() 
        self.V = Vec_xyz() #< u, v, w  linear Velocity 
        self.A = Vec_xyz() #< u, v, w  linear Acceleration   
        self.F = Vec_xyz() #< u, v, w  linear Force (forward, right, down )

        self._q = Qtrn4f(1.0, 0.0, 0.0, 0.0) #< Initial orientation    
        #self.attitude = Attitude()
        self.Ar = Vec_pqr()   #< Roll (x), Pitch (y), Yaw (z) angular position
        self.W = Vec_pqr() #< p, q, r (xyz) angular velocity
        self.W_dot = Vec_pqr() #< p_dot, q_dot, r_dot (xyz) angular accel
        self.L = Vec_pqr() #< L, M, N (xyz) angular moment
        self.T = Vec_pqr() #< L, M, N (xyz) Torque
    
##===========================================================================================================

        #self.Lm, self.Mm, self.Nm = (0.0, 0.0, 0.0) #< L, M, N (xyz) angular moment
        #self.Ap, self.Aq, self.Ar = (0.0, 0.0, 0.0) #< p_dot, q_dot, r_dot (xyz) angular accel
        #self.Pp, self.Pq, self.Pr = (0.0, 0.0, 0.0) #< Roll (x), Pitch (y), Yaw (z) angular position

        self.alpha_r = 0.0  #< Angle of attack 
        self.beta_r = 0.0   #< Sideslip angle 
        self.Lift, self.Drag = (0.0, 0.0)
        self.Altitude_m  = altInit_m

        self.Wind = Vec3()        #< wind vector [m/s] 

        self.Weight = weight_lbs  #< Weight lbs : mass (lbs/G slugs )
        self.Thrust = 0.0 

        self.delta_e_deg = 0.0    #< Elevator deflection (degrees)
        self.delta_e = 0.0        #< Elevator deflection (radians)
        self.delta_e_trim = 0.0

        self.delta_a_deg = 0.0    #< Aileron deflection (degrees)
        sself.delta_a = 0.0       #< Aileron deflection (radians)

        self.delta_r_deg = 0.0    #< Rudder deflection (degrees)
        self.delta_r = 0.0        #< Rudder deflection (radians)

        if (units == "Metric") or (units == "Imperial"):    
            print("UNITS set to %s\n"%units)
        else:
            print(">>>>> Units Not defined %s\n"%units)  
       

    def step(self, ctrls, dt=None ):
        dt = self.dt if dt==None else dy
        self.time += dt
        
        ##================ Controls ================================================================================================
        self.delta_e_deg  = -(ctrls.Elevator_Cmd * self.params.ELV_MAX_ANG )      #< Pitch stick y axis range -1.0 to 1.0
        self.delta_e_trim = self.params.ELV_TRIM
        self.delta_e      = (self.delta_e_deg + self.delta_e_trim) * DEGtoRAD

        self.delta_a_deg  = (ctrls.Aileron_Cmd * self.params.AIL_MAX_ANG ) / 5.0  #< Roll stick x axis range -1.0 to 1.0
        self.delta_a      = self.delta_a_deg * DEGtoRAD

        self.delta_r_deg  = (ctrls.Rudder_Cmd * self.params.RUD_MAX_ANG )         #< Roll stick x axis range -1.0 to 1.0
        self.delta_r      = self.delta_r_deg * DEGtoRAD

        self.Thrust = ctrls.Throttle_Cmd * self.params.MAX_THRUST                 #< Throttle Command setting [ 0, 1]
        ##================== End Controls ============================================================================================

        ##================== Airspeed, Alpha, Beta, Flight Path ======================================================================
        ## Update the airspeed 
        if abs(self.X.x) < 1.0:
            self.alpha_r = 0.0     #< No AOA and Sideslip at low speed
            self.beta_r  = 0.0
        else:
            self.alpha_r = math.atan2(  self.V.z, self.X.x ) #< Trajactor vs air-speed vectors
            self.beta_r  = math.atan2( -self.V.y, self.V.x ) #< Trajactor vs air-speed vectors

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

        Vabs = self.V.abs() #<math.sqrt(Vx**2 +Vy**2 +Vw**2)
        Wp, Wq, Wr = self.W

        ##=======================================================================================================================
        ## Momenets and rotation
        ## X axis
        self.T.p = qSb * ( Clo +( Clda * self.delta_a ) +( Clp*Wp*_B/(2*Vabs)) +( Clr*Wr*_B/(2*Vabs)))
        Ap = self.T.p / self.parames._Ixx ## calc roll rate radians/sec. (Torue / Moment_Inertia) * time 
        Wp += Ap * dt
        
        ## Y axis
        self.T.q = qSc * ( Cmde * self.delta_e +Cma * self.alpha_r +Cmq * Wq )
        Aq = self.T.q / self.parames._Iyy ## calc pitch rate radians/sec. (Torque / Moment_Inertia) * time     
        Wq += Aq * dt

        ## Z axis 
        self.T.r = qSb * ( Cno +(Cnb * self.beta_r) + (Cnp * Wp*self.parames._B/(2*Vabs)) +( Cnr * Wr*self.parames._B/(2*Vabs) +( Cndr * self.delta_r ) ))
        Ar = self.T.r / self.parames._Izz      ## calc yaw rate radians/sec. (Torque / Moment_Inertia) * time
        Wr += Ar * dt
        ##=======================================================================================================================   

        ## Lift and drag forces - linear
        CLo   = self.parames.CL_0
        CLa   = self.parames.CL_ALPHA     
        CDo   = self.parames.CD_0
        Cyb   = self.parames.CY_B
        Cydr  = self.parames.CY_DELTA_R
        Cyp   = self.parames.CY_p
        Cyr   = self.parames. CY_r

        CL = ( CLo + ( CLa * self.alpha_r ))
        Cd = ( CDo + ( self.parames.K*(CL**2) ))
        
        self.Lift = qS * CL
        self.Drag = qS * Cd
        ##===============================================================   

        ## X Axis
        self.F.x = self.Lift * math.sin(self.alpha_r)\
                  -self.Drag * math.cos(self.alpha_r)\
                  +self.Thrust\
                  -self.Weight * math.sin(self.Pitch)
        Au = self.F.x / self.parames.MASS  
        self.V.x += Au * dt

        ## Y Axis
        Cy = Cyb * self.beta_r\
             +Cydr * self.delta_r\
             +Cyp * Wp * self.V.z / Vabs\
             +Cyr * Wr * self.V.z / Vabs

        self.F.y = qS * Cy
        Av = self.F.y / self.params.MASS\
                  +Wr * self.V.x\
                  -Wp * self.V.z\
                  +self.parames.G * math.cos(self.attitude.pitch_r) * math.sin(self.attitude.roll_r)
        self.V.y += Av * dt
    
        ## Z axis, (-) to flip for Z axis sign convention, right hand rule
        self.F.z = self.Lift*math.cos(self.alpha_r)\
                  -self.Drag * math.sin(self.alpha_r)\
                  -self.Weight*math.cos(self.attitude.roll_r)*math.cos(self.attitude.pitch_r)
        self.F.z *= -1
        Aw = self.F.z / self.parames.MASS
        self.V.z += Aw * dt
        ##=======================================================================================================================   

        ## Body to Intertial transform
        d_q = Qtrn4()
        Vi_inertial = Vec3()

        self.W.x = Wp
        self.W.y = Wq
        self.W.z = Wr

        ## Quaternion process, next 5 lines
        d_q = quat_from_ang_rates( self.W, dt )   
        self._q.multiply(d_q )
        self._q.normalize_quat()
        Vi_inertial = rotate_body_to_inrtl( self.Vinf_V, self._q ) ##< ???       
        self.Att.roll_r, self.Att.pitch_r, self.Att.yaw_r = self._q.Qtrn_to_Euler_deg()

        Vinf_V.x = Vx
        Vinf_V.y = Vy

        ## 6DOF inertial solution       
        self.position.x += Vi_inertial.x * dt
        self.position.y += Vi_inertial.y * dt
        self.position.z -= Vi_inertial.z * dt
        ##=================================================================================================================

    def getAttitude(self):
        """Return Vector3( roll_rad, pitch_rad, yaw_rad )"""
        return Vec3(self.roll_r, self.pitch_r, self.yaw_r)

    def getPosition(self):
        """Return Vector3(x, y, z)"""
        return Vec3(self.position.x, self.position.y, self.position.z)

##=================================================================================================================
## ChatGTP Quaternion Code

def T_Body_to_Inrt_Vel( V_body, q ): 
    """Rotate body Vel to inrt frame"""
    x = q.x
    y = q.y
    z = q.z
    w = q.w;

    R11 = 1 - 2 * ( y * y + z * z );
    R12 = 2     * ( x * y - z * w );
    R13 = 2     * ( x * z + y * w );

    R21 = 2     * ( x * y + z * w );
    R22 = 1 - 2 * ( x * x + z * z );
    R23 = 2     * ( y * z - x * w );

    R31 = 2     * ( x * z - y * w );
    R32 = 2     * ( y * z + x * w );
    R33 = 1 - 2 * ( x * x + y * y );

    return Vec_xyz(
        R11 * V_body.x + R12 * V_body.y + R13 * V_body.z,
        R21 * V_body.x + R22 * V_body.y + R23 * V_body.z,
        R31 * V_body.x + R32 * V_body.y + R33 * V_body.z)

## Create delta quaternion from angular Vi

def rotate_body_to_inrtl( body, q ):
    """Rotate body vector to inertial frame using quaternion"""
    p = Qtrn(w=0, x=body.x, y=body.y, z=body.z)
    q.quat_multiply( p ).multiply( q_conj )
    return Vec_xyz(x=rotated.x, y=rotated.y, z=rotated.z)
 
## End - ChatGTP Quaternion Code
##================================================================================================================= 
