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

        self.position = Vec_xyz(0.0, 0.0, -altInit_m) 
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
        self.elevatorCmd  = -(ctrls.Elevator_Cmd * self.params.ELV_MAX_ANG_D ) #< Pitch stick y axis range -1.0 to 1.0
        self.elevatorCmd += self.elevatorTrim_deg
        self.elevatorCmd *= DEGtoRAD

        self.aileronCmd  = -(ctrls.Aileron_Cmd * self.params.AIL_MAX_ANG_D )   #< Roll stick x axis range -1.0 to 1.0
        self.aileronCmd += self.aileronTrim_deg
        self.aileronCmd *= DEGtoRAD

        self.rudderCmd  = -(ctrls.Rudder_Cmd * self.params.RUD_MAX_ANG_D )     #< Roll stick x axis range -1.0 to 1.0
        self.rudderCmd += self.rudderTrim_deg
        self.rudderCmd *= DEGtoRAD

        self.Thrust = ctrls.Throttle_Cmd * self.params.MAX_THRUST              #< Throttle Command setting [ 0, 1]
        ##================== End Controls ============================================================================================

        ##================== Airspeed, Alpha, Beta, Flight Path ======================================================================
        ## Update the airspeed 
        Vabs = self.Vb.mag()

        ### Where is flight path ??
        ## V earth to body
        #self.Vb = Vec_xyz(*MxV(self.attitude.inv(), self.Ve.getVector()))
        if abs(self.Vb.x) < 1.0:
            self.alpha_r = 0.0     #< No AOA and Sideslip at low speed
            self.beta_r  = 0.0
        else:
            self.alpha_r =  atan( self.Vb.z/self.Vb.x ) #< GSOF - Probably incorrect
            self.beta_r  = -asin( self.Vb.y/Vabs )      #< GSOF - Probably incorrect
        ##============================================================================================================================

        ##========================== Momenets and rotation============================================================================
        qS  = 0.5 * self.params.RHO * (Vabs**2) * self.params._S
        _B  = self.params._B
        qSc = qS * self.params._C
        qSb = qS * _B
        Wp, Wq, Wr = self.W.p, self.W.q, self.W.r #< Current state

        ## X axis
        Clo   = self.params.Cl_0       #< Roll, Zero-control moment ( typically small or zero in symmetric flight )
        Clda  = self.params.Cl_DA      #< Roll, Aileron effectiveness 9 change in Cl per radian of aileron deflection )
        Clr   = self.params.Cl_R       #< Roll, Yaw-roll coupling ( change in Cl per unti of yaw rate. )
        Clp   = self.params.Cl_P       #< Roll, Damping ( change in Cl per unit of roll rate ) 
        self.T.p  = Clda*self.aileronCmd     #< Roll command
        self.T.p += Clo -0.05*self.rudderCmd #< Roll moment due to rudder command
        self.T.p += Clr*Wr*_B/(2*Vabs)       #< Yaw rate to roll moment
        self.T.p += Clp*Wp*_B/(2*Vabs)       #< Rate resistance
        self.T.p *= qSb                      #< Factor due to air speed 
        
        ## Y axis
        Cmo   = self.params.CM_0       #< Baseline pitching moment coefficient
        Cmde  = self.params.CM_DELTA_E #< Pitching moment slope due to elevator deflection (per radian)
        Cma   = self.params.CM_ALPHA   #< Pitching moment slope due to AoA (per radian)
        Cmq   = self.params.CM_Q       #< Pitch Damping coefficient#-0.7, -0.15
        self.T.q  = Cmde*self.elevatorCmd #< Elevon command
        self.T.q += Cmo +Cma*self.alpha_r #< Wing pitch moment (baseline and angle of attack)
        self.T.q += Cmq*Wq                #< Rate resistance
        self.T.q *= qSc                   #< Factor due to air speed 
        
        ## Z axis 
        Cno   = self.params.CN_0       #< Baseline yaw moment coefficient
        Cnb   = self.params.CN_b       #< Yaw, sideslip moment, yaw stability
        Cnp   = self.params.CN_p       #< Yaw, roll moment, rikk-yaw coupling 
        Cndr  = self.params.CN_dr      #< Yaw, rudder deflection moment, rudder effectiveness
        Cnda  = self.params.CN_da      #< Yaw, aileron defection moment, aileron inducted yaw
        Cnr   = self.params.CN_r       #< Yaw, Damping coefficient
        self.T.r  = Cndr*self.rudderCmd   #< Elevon command
        self.T.r += Cno +Cnb*self.beta_r  #< Ruddermoment baseline and beta angle
        self.T.r += Cnp*Wp*_B/(2*Vabs)    #< Roll rate to Yaw moment
        self.T.r += Cnr*Wr*_B/(2*Vabs)    #< Rate resistance
        self.T.r *= qSb                   #< Factor due to air speed 
        ##=======================================================================================================================   

        ## Lift and drag forces - linear
        CL = self.params.CL_0 +self.params.CL_ALPHA*self.alpha_r
        self.Lift = qS * CL
        Cd = self.params.CD_0 +self.params.K*(CL**2)
        self.Drag = qS * Cd
        ##===============================================================   

        ## X Axis
        self.Fb.x  = self.Thrust
        self.Fb.x +=  self.Lift * sin(self.alpha_r)
        self.Fb.x += -self.Drag * cos(self.alpha_r)
        self.Fb.x += -self.Weight * sin(self.attitude.pitch_r) #< Forward force due to gravity and pitch angle
        self.Ab.x = self.Fb.x / self.params.MASS  

        ## Y Axis
        Cy = self.params.CY_B * self.beta_r
        Cy += self.params.CY_DELTA_R*self.rudderCmd
        Cy += self.params.CY_p*Wp*self.Vb.z/(2*Vabs)
        Cy += self.params.CY_r*Wr*self.Vb.z/(2*Vabs)

        self.Fb.y = qS * Cy
        self.Ab.y  = self.Fb.y / self.params.MASS
        self.Ab.y += Wr * self.Vb.x #< Side force due to roll rate
        self.Ab.y += Wp * self.Vb.z #< Side force due to pitch rate
        self.Ab.y += self.params.G * cos(self.attitude.pitch_r) * sin(self.attitude.roll_r) #< Side force due to gravity

        ## Z axis, (-) to flip for Z axis sign convention, right hand rule
        self.Fb.z =   self.Lift * cos(self.alpha_r)
        self.Fb.z += -self.Drag * sin(self.alpha_r)
        self.Fb.z += -self.Weight * cos(self.attitude.roll_r) * cos(self.attitude.pitch_r)
        self.Fb.z *= -1
        self.Ab.z = self.Fb.z / self.params.MASS
        ##=======================================================================================================================   

        ##========================== 6-DOF SOLVER ===============================================================================   
        ## Angular velocity integration in body coordinates
        self.W.p += (self.T.p / self.params._Ixx) * dt #< Next state
        self.W.q += (self.T.q / self.params._Iyy) * dt #< Next state
        self.W.r += (self.T.r / self.params._Izz) * dt #< Next state
        self.attitude.addW(self.W, dt)

        ## Body to earth transform
        self.Vb.x += self.Ab.x * dt #< Next state
        self.Vb.y += self.Ab.y * dt #< Next state
        self.Vb.z = self.Ab.z * dt #< Next state
        self.Ve = Vec_xyz(*MxV(self.attitude.dcm, self.Vb.getVector()))

        ## Position in earth coordinates
        self.position.x += self.Ve.x * dt
        self.position.y += self.Ve.y * dt
        self.position.z += self.Ve.z * dt #< Z axis is pointing down

        print("AOA,Beta: %1.2f, %1.2f"%(self.alpha_r*RADtoDEG, self.beta_r*RADtoDEG))
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
