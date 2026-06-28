
from mathLib import *

class Solver_6DOF():
    def __init__(self, position, Vb, Ve, mass, attitude, Wb, inertia):
        self.position = position
        self.Vb = Vb
        self.Ve = Ve
        self.attitude = attitude
        self.Wb = Wb
        self.I = inertia
        self.mass = mass
        
    def step(self, Fb, Tb, dt, Fe=Vec_xyz(0,0,0)) -> list:
        ## Next state - Angular velocity integration in body coordinates
        self.Wb.p += (Tb.p / self.I.x) * dt
        self.Wb.q += (Tb.q / self.I.y) * dt
        self.Wb.r += (Tb.r / self.I.z) * dt

        self.attitude.addW(self.Wb, dt)

        ## Next state - Body to earth transform
        mass = self.mass[0]
        self.Vb.x += (Fb.x/mass) * dt
        self.Vb.y += (Fb.y/mass) * dt
        self.Vb.z += (Fb.z/mass) * dt
        Ve = MxV(self.attitude.dcm, self.Vb.getVector())
        self.Ve.x, self.Ve.y, self.Ve.z = (Ve[0], Ve[1], Ve[2])

        ## Next state - Position in earth coordinates
        self.position.x += self.Ve.x * dt
        self.position.y += self.Ve.y * dt
        self.position.z += self.Ve.z * dt
