##==============================================================================
##
##       SIMULATION Function,  Flightmodel-810-03
##
##==============================================================================

from math import sin, cos, atan
from mathLib import *
import NavionAircraftParameters as airMdl

class Controls():
    def __init__(self, Elevator_Cmd, Aileron_Cmd, Rudder_Cmd, Throttle_Cmd, GearExtend_Cmd):
        self.Elevator_Cmd = Elevator_Cmd
        self.Aileron_Cmd  = Aileron_Cmd
        self.Rudder_Cmd   = Rudder_Cmd
        self.Throttle_Cmd = Throttle_Cmd
        self.GearExtend_Cmd = int(GearExtend_Cmd)
        
class AeroModel():
    def __init__(self, dt, altInit_m, speed_fps, weight_lbs, units):
        self.params = airMdl.NavionParams()
        self.dt = dt
        self.time = 0.0

        self.attitude = Attitude(0,0,0) #< Initial orientation
        self.W = Vec_pqr()              #< p, q, r (xyz) angular velocity
        self.T = Vec_pqr()              #< Torque

        self.position = Vec_xyz(0.0, 0.0, altInit_m) 
        self.Vb = Vec_xyz(speed_fps, 0.0, 0.0) #< u, v, w  linear Velocity 
        self.Ve = Vec_xyz(*MxV(self.attitude.dcm, self.Vb.getVector()))
        self.Ab = Vec_xyz() #< Acceleration (u, v, w)
        self.Fb = Vec_xyz() #< Force (forward, right, down )
    
        self.alpha_r = 0.0  #< Angle of attack 
        self.beta_r = 0.0   #< Sideslip angle 
        self.Lift, self.Drag = (0.0, 0.0)

        self.Weight = weight_lbs  #< Weight lbs : mass (lbs/G slugs )
        self.Thrust = 0.0 

        self.elevatorCmd = 0.0    #< Elevator deflection (radians)
        self.elevatorTrim_deg = self.params.ELV_TRIM_D

        self.aileronCmd = 0.0    #< Aileron deflection (radians)
        self.aileronTrim_deg = self.params.AIL_TRIM_D

        self.rudderCmd = 0.0    #< Rudder deflection (degrees)
        self.rudderTrim_deg = self.params.RUD_TRIM_D

        if (units == "Metric") or (units == "Imperial"):    
            print("UNITS set to %s\n"%units)
        else:
            print(">>>>> Units Not defined %s\n"%units)  

    def step(self, ctrls, dt=None ):
        dt = self.dt if dt==None else dt
        self.time += dt
        
        ##================ Controls ================================================================================================
        self.elevatorCmd  = (ctrls.Elevator_Cmd * self.params.ELV_MAX_ANG_D ) #< Pitch stick y axis range -1.0 to 1.0
        self.elevatorCmd += self.elevatorTrim_deg
        self.elevatorCmd *= DEGtoRAD

        self.aileronCmd  = (ctrls.Aileron_Cmd * self.params.AIL_MAX_ANG_D )    #< Roll stick x axis range -1.0 to 1.0
        self.aileronCmd += self.aileronTrim_deg
        self.aileronCmd *= DEGtoRAD

        self.rudderCmd  = (ctrls.Rudder_Cmd * self.params.RUD_MAX_ANG_D )      #< Roll stick x axis range -1.0 to 1.0
        self.rudderCmd += self.rudderTrim_deg
        self.rudderCmd *= DEGtoRAD

        self.Thrust = ctrls.Throttle_Cmd * self.params.MAX_THRUST              #< Throttle Command setting [ 0, 1]
        ##================== End Controls ============================================================================================

        ##================== Airspeed, Alpha, Beta, Flight Path ======================================================================
        ## Update the airspeed 
        if abs(self.Vb.x) < 1.0:
            self.alpha_r = 0.0     #< No AOA and Sideslip at low speed
            self.beta_r  = 0.0
        else:
            self.alpha_r = atan(  self.Vb.z/self.Vb.x ) #< GSOF - Probably incorrect
            self.beta_r  = atan( -self.Vb.y/self.Vb.x ) #< GSOF - Probably incorrect

        Vabs = self.Vb.mag()
        qS  = 0.5 * self.params.RHO * (Vabs**2) * self.params._S
        _B  = self.params._B
        qSc = qS * self.params._C
        qSb = qS * _B
        ##============================================================================================================================
        
        Cmo   = self.params.CM_0
        Cmq   = self.params.CM_Q 
        Cmde  = self.params.CM_DELTA_E
        Cma   = self.params.CM_ALPHA
        
        Clo   = self.params.Cl_0     #< Roll, Zero-control moment ( typically small or zero in symmetric flight )
        Clda  = self.params.Cl_DA    #< Roll, Aileron effectiveness 9 change in Cl per radian of aileron deflection )
        Clp   = self.params.Cl_P     #< Roll, Damping ( change in Cl per unit of roll rate ) 
        Clr   = self.params.Cl_R     #< Roll, Yaw-roll coupling ( change in Cl per unti of yaw rate. )
        
        Cno   = 0.0;
        Cnb   = self.params.CN_b     #< Yaw, sideslip moment, yaw stability
        Cnp   = self.params.CN_p     #< Yaw, roll-rate moment, rikk-yaw coupling 
        Cnr   = self.params.CN_r     #< Yaw, yaw-rate moment, yaw damping
        Cndr  = self.params.CN_dr    #< Yaw, rudder deflection moment, rudder effectiveness
        Cnda  = self.params.CN_da    #< Yaw, aileron defection moment, aileron inducted yaw

        Wp, Wq, Wr = self.W.p, self.W.q, self.W.r #< Current state

        ##=======================================================================================================================
        ## Momenets and rotation
        ## X axis
        self.T.p  = qSb * ( Clo +Clda*self.aileronCmd +Clp*Wp*_B/(2*Vabs) +Clr*Wr*_B/(2*Vabs))
        Ap        = self.T.p / self.params._Ixx ## calc roll rate radians/sec. (Torue / Moment_Inertia) * time 
        self.W.p += Ap * dt #< Next state
        
        ## Y axis
        self.T.q  = qSc * ( Cmde*self.elevatorCmd +Cma*self.alpha_r +Cmq*Wq )
        Aq        = self.T.q / self.params._Iyy ## calc pitch rate radians/sec. (Torque / Moment_Inertia) * time     
        self.W.q += Aq * dt #< Next state

        ## Z axis 
        self.T.r  = qSb * ( Cno +Cnb*self.beta_r +Cnp*Wp*_B/(2*Vabs) +Cnr*Wr*_B / (2*Vabs) +Cndr*self.rudderCmd )
        Ar        = self.T.r / self.params._Izz ## calc yaw rate radians/sec. (Torque / Moment_Inertia) * time
        self.W.r += Ar * dt #< Next state
        ##=======================================================================================================================   

        ## Lift and drag forces - linear
        CLo   = self.params.CL_0
        CLa   = self.params.CL_ALPHA     
        CDo   = self.params.CD_0
        Cyb   = self.params.CY_B
        Cydr  = self.params.CY_DELTA_R
        Cyp   = self.params.CY_p
        Cyr   = self.params.CY_r

        CL = ( CLo +( CLa * self.alpha_r ))
        Cd = ( CDo +( self.params.K*(CL**2) ))
        
        self.Lift = qS * CL
        self.Drag = qS * Cd
        ##===============================================================   

        ## X Axis
        self.Fb.x = self.Lift * sin(self.alpha_r)\
                  -self.Drag * cos(self.alpha_r)\
                  +self.Thrust\
                  +self.Weight * sin(self.attitude.pitch_r)
        self.Ab.x = self.Fb.x / self.params.MASS  
        self.Vb.x += self.Ab.x * dt #< Next state

        ## Y Axis
        Cy = Cyb * self.beta_r\
             +Cydr * self.rudderCmd\
             +Cyp * Wp * self.Vb.z / (2*Vabs)\
             +Cyr * Wr * self.Vb.z / (2*Vabs)

        self.Fb.y = qS * Cy
        self.Ab.y = self.Fb.y / self.params.MASS\
                  +Wr * self.Vb.x\
                  -Wp * self.Vb.z\
                  +self.params.G * cos(self.attitude.pitch_r) * sin(self.attitude.roll_r)
        self.Vb.y += self.Ab.y * dt #< Next state
    
        ## Z axis, (-) to flip for Z axis sign convention, right hand rule
        self.Fb.z = self.Lift * cos(self.alpha_r)\
                  -self.Drag * sin(self.alpha_r)\
                  -self.Weight * cos(self.attitude.roll_r) * cos(self.attitude.pitch_r)
        self.Fb.z *= -1
        self.Ab.z = self.Fb.z / self.params.MASS
        self.Vb.z = self.Ab.z * dt #< Next state
        ##=======================================================================================================================   

        ## Angular velocity integration in body coordinates
        self.attitude.addW(self.W, dt)

##        ## V earth to body
##        Vb = Vec_xyz(*MxV(self.attitude.inv(), self.Ve.getVector()))
##
##        ## Apply body accel
##        self.Vb.x += Vb.x +self.Ab.x*dt
##        self.Vb.y += Vb.y +self.Ab.y*dt
##        self.Vb.z += Vb.y +self.Ab.z*dt
##
##        ## Body to earth transform
##        self.Ve = Vec_xyz(*MxV(self.attitude.dcm, self.Vb.getVector()))

        ## Body to earth transform
        self.Ve = Vec_xyz(*MxV(self.attitude.dcm, self.Vb.getVector()))
        self.position.x += self.Ve.x * dt
        self.position.y += self.Ve.y * dt
        self.position.z += self.Ve.z * dt #< Z axis is pointing down

        #print("AOA,Beta: %1.2f, %1.2f"%(self.alpha_r*RADtoDEG, self.beta_r*RADtoDEG))
        #print("Torque : %s"%(self.T))
        #print("Vb: %s"%(self.Vb))
        #print("Ve: %s"%(self.Ve))

        ##=================================================================================================================

    def __str__(self):
        s = "="*10 +" %1.2f sec "%(self.time) +"="*10 +"\n"
        Pos = self.position
        s += "X-Forward; Y-Right; Z-Down\n"
        s += "Position    :, %1.2f, %1.2f, %1.2f\n"%(   Pos.x,    Pos.y,    Pos.z)
        s += "Velosity    :, %s1.2f, %1.2f, %1.2f\n"%(self.Vb.x, self.Vb.y, self.Vb.z)
        s += "Accel       :, %1.2f, %1.2f, %1.2f\n"%(self.Ab.x, self.Ab.y, self.Ab.z)
        s += "Ele,Ail,RudA:, %1.2f, %1.2f, %1.2f\n"%(self.elevatorCmd*RADtoDEG, self.aileronCmd*RADtoDEG, self.rudderCmd*RADtoDEG)
        s += "Lift,Drag   :, %1.2f, %1.2f\n"%(self.Lift, self.Drag)
        s += "AOA,Beta    :, %1.2f, %1.2f\n"%(self.alpha_r, self.beta_r)

        Att = self.attitude
        s += "\n"
        s += "p-Roll; q-Pitch; r-Yaw\n"
        s += "Attitude :, %1.1f, %1.1f, %1.1f\n"%(Att.roll_r, Att.pitch_r, Att.yaw_r)
        s += "Omega    :, %1.3f, %1.3f, %1.3f\n"%(self.W.p, self.W.q, self.W.r)
        #s += "Omega_dot:, %1.3f, %1.3f, %1.3f\n"%(self.W_dot.p, self.W_dot.q, self.W_dot.r)
        s += "Torque   :, %1.2f, %1.2f, %1.2f\n"%(self.T.p, self.T.q, self.T.r)
        return s

    def print(self):
        print(self)

    def getAttitude(self):
        """Return Vector3( roll_rad, pitch_rad, yaw_rad )"""
        return Vec3(self.roll_r, self.pitch_r, self.yaw_r)

    def getPosition(self):
        """Return Vector3(x, y, z)"""
        return Vec3(self.position.x, self.position.y, self.position.z)


if __name__ == "__main__":
    mdl = AeroModel(dt=0.1, altInit_m=0.0, speed_fps=210.0, weight_lbs=2750, units="Metric")
    ctrl = Controls(Elevator_Cmd= 0.9, Aileron_Cmd=-0.0, Rudder_Cmd=.0, Throttle_Cmd=0.0, GearExtend_Cmd=0.0)
    mdl.print()
    for i in range(0,20):
        mdl.step(ctrl, dt=0.1)
        mdl.print()
