"""
 * TelemetryRx.py
 * Created on: 8 April 2026
 * Author: Guy Soffer
 * Copyright (C) 2026 Guy Soffer
"""

import math, sys, time
import pygame
from GSOF_Cockpit.GraphicsLib import getMouse

from bus.BusINS import *
from bus.BusFcsCmds import *
from flightmodel import AeroModel, Controls

from math import pi
radToDeg = 180/pi

class Wow():
    def __init__(self):
        self.left  = False
        self.right = False
        self.nose  = False

    def print(self):
        print("%d, %d, %d"%(self.left, self.nose, self.right))

class Buses():
    def __init__(self):
        self.mousePos     = (0,0)
        self.mousePos_Z1  = self.mousePos

        self.ins    = BusINS()
        self.cmds   = BusFcsCmds()
        self.time   = 0.0
        self.wow    = Wow()
        self.ctrls = Controls(
            Elevator_Cmd =  0.00,
            Aileron_Cmd  =  0.00,
            Rudder_Cmd   =  0.00,
            Throttle_Cmd =  0.00,
            GearExtend_Cmd = 0)
    
class Data():
    """Data source to drive gauges screen"""
    def __init__(self, screen_size):
        self.scrSize = screen_size
        self.scrCenter = (self.scrSize[0]/2, self.scrSize[1]/2)
        self.height_Z1 = 0.0
        self.time_Z1 = time.time()
        self.dt = 0.1
        self.data = Buses()
        self.navion = AeroModel(dt=0.05, altInit_m=300.0, speed_fps=220.0, weight_lbs=2750, units="Metric")

    def getData(self, test=False):
        """Generate and return new set of data"""
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                print('Exiting....')
                sys.exit()   # end program.
        t = time.time()
        self.dt = t -self.time_Z1
        self.time_Z1 = t
        m_data = self.data
        cmds = m_data.cmds
        
        if test != False:
            # Interactive test mode (ARCADE)
            m_data.time += self.dt
            keys = pygame.key.get_pressed()

            ### FLIGHT CONTROLS AND THROTTLE
            m_data.mousePos_Z1 = m_data.mousePos
            m_data.mousePos = (getMouse())["pos"]
            roll = -(m_data.mousePos[0]/self.scrSize[0] -0.5)
            elev = -(m_data.mousePos[1]/self.scrSize[1] -0.5)
            rud  = -roll
            sb   = 0.5
            throttle = cmds.throttleCmd +0.01*(keys[pygame.K_q] -keys[pygame.K_a])

            if test == "navion":
                self.data.ctrls.Elevator_Cmd = -0.5*elev
                self.data.ctrls.Aileron_Cmd  = 0.5*roll
                self.data.ctrls.Rudder_Cmd   = rud
                self.data.ctrls.Throttle_Cmd = throttle

                ### 6DOF MODEL
                self.navionPhysics()
                
            elif test == "arcade":
                rudder     = -2*roll
                speedbrake =  60*0.5
                cmds.lElevonCmd_d = 45*( roll +elev)
                cmds.rElevonCmd_d = 45*(-roll +elev)
                cmds.rudderCmd_d  = rudder
                cmds.speedbrakeCmd_d = speedbrake
                cmds.throttleCmd = max(0, min(1, throttle))

                ### 6DOF MODEL
                self.arcadePhysics()

            else:
                print("Incorrect test mode %s"%test)

            ### LANDING GEARS
            gearsDown = cmds.gearExtendCmd_b +(keys[pygame.K_b] -keys[pygame.K_g])
            cmds.gearExtendCmd_b = max(0, min(1, gearsDown))

            ### WEIGHT ON WHEELS
            m_data.wow.left  = bool(keys[pygame.K_1])
            m_data.wow.nose  = bool(keys[pygame.K_2])
            m_data.wow.right = bool(keys[pygame.K_3])

            ### Weight On Wheels (WOW) detection
            m_data.wow.left   |= (m_data.ins.height < 0.1) and (m_data.ins.roll < 0.5)
            m_data.wow.right  |= (m_data.ins.height < 0.1) and (m_data.ins.roll > -0.5)
            m_data.wow.nose   |= (m_data.ins.height < 0.1) and (m_data.ins.pitch < 0.5)

        else:
            # Telemetry mode (PLAYBACK)
            print("Playback mode isn't supported yet")
        return m_data

    def navionPhysics(self) -> None:
        m_data = self.data
        ins = m_data.ins
        cmds = m_data.cmds
        mdl = self.navion
        mdl.step(m_data.ctrls, dt=self.dt) #, dt=self.dt)

#        mdl.attitude.print()

        ins = m_data.ins
        ins.roll    =  radToDeg*mdl.attitude.roll_r
        ins.pitch   =  radToDeg*mdl.attitude.pitch_r
        ins.azimuth = -radToDeg*mdl.attitude.yaw_r

        ins.height  = -mdl.position.z
#        print("ALT: ", m_data.ins.height)

        airSpeed = mdl.V.mag()
        ins.vel_up    = -mdl.V.z
#        print("VSI: ", m_data.ins.vel_up)
        ins.vel_north = airSpeed*math.sin(ins.azimuth)
        ins.vel_east  = airSpeed**math.cos(ins.azimuth)

        ins.upAcc      = -mdl.A.z
        ins.forwardAcc =  mdl.A.x
        ins.rightAcc   =  mdl.A.y

        ins.mach = airSpeed*0.002

        ### Ground collision detection
        if ins.height < 0:
            mdl.position.z = 0.0

            if ins.pitch < 0.0:
                mdl.attitude.set( 0,0,0 )
                mdl.V.z = 0.0
                mdl.W.p = 0.0
                mdl.W.q = 0.0
                
    def arcadePhysics(self) -> None:
        m_data = self.data
        ins = m_data.ins
        cmds = m_data.cmds

        ### ARCADE "6DOF"
        thrustFactor = cmds.throttleCmd
        fcsRoll  = -0.5*(cmds.lElevonCmd_d -cmds.rElevonCmd_d)
        fcsPitch = 0.5*(cmds.lElevonCmd_d +cmds.rElevonCmd_d)
        noseUpTorque   = -fcsPitch*thrustFactor
        rollLeftTorque = fcsRoll*thrustFactor
        ins.roll  = rollLeftTorque
        ins.pitch = noseUpTorque
        ins.azimuth += 0.01*m_data.ins.roll
        ins.height  += 0.02*m_data.ins.pitch
        ins.height -= (1-thrustFactor) #< Sink vs "speed"
        ins.vel_up = (ins.height -self.height_Z1) / self.dt
        ins.vel_north = 800*thrustFactor*math.sin(ins.azimuth)
        ins.vel_east  = 800*thrustFactor*math.cos(ins.azimuth)
        self.height_Z1  = ins.height  #< VSI
        ins.mach = 1.5*thrustFactor   #< MACH
        cmds.gearExtendCmd_b |= int(ins.height < 100) #< Gears down if below 100 m
        ins.upAcc = 9.81 +0.5*ins.vel_up
        ins.forwardAcc = 0.0
        ins.rightAcc = 2*ins.roll
        
        ### Ground collision detection
        if ins.height <= 0:
            ins.height = 0.0
            if ins.pitch < 0.0:
                ins.pitch = 0.0
