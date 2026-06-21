import math
from typing import Self, TypeVar

Q = TypeVar('Q', bound='Qtrn')
V_xyz = TypeVar('V_xyz', bound='Vec_xyz')
V_pqr = TypeVar('V_pqr', bound='Vec_pqr')

PI = math.pi
RADtoDEG = 180/PI
DEGtoRAD = PI/180

def Sign( x ):
    if  x  < 0 :
        return -1
    return 1

class Vec_xyz():
    def __init__(self, x=0,y=0,z=0) -> Self:
        self.set(x,y,z) #< Forward, Right, Down

    def set(self, x, y, z) -> Self:
        self.x=x #< Forward
        self.y=y #< Right
        self.z=z #< Down
        return self
        
    def copy(self) -> Self:
        return Vec_xyz(self.x, self.y, self.z)
        
    def mag(self) -> float:
        return math.sqrt(self.x**2 +self.y**2 +self.z**2)

    def __str__(self) -> str:
        s = ""
        for v in (self.x, self.y, self.z):
            s += "%1.3f, "%(v)
        return s[0:-2]

    def print(self) -> None:
        print("Vec_xyz:, " +self.__str__())

class Vec_pqr():
    def __init__(self, p=0,q=0,r=0) -> Self:
        self.set(p, q, r) #< Roll, Pitch, Yaw
        
    def set(self, p, q, r) -> Self:
        self.p=p #< Roll
        self.q=q #< Pitch
        self.r=r #< Yaw
        return self

    def copy(self) -> Self:
        return Vec_pqr(self.p, self.q, self.r)
        
    def mag(self) -> float:
        return math.sqrt(self.p**2 +self.q**2 +self.r**2)

    def getQuaternion(self, dt ) -> Q:
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

    def __str__(self) -> str:
        s = ""
        for v in (self.p, self.q, self.r):
            s += "%1.3f, "%(v)
        return s[0:-2]

    def print(self) -> None:
        print("Vec_pqr:, " +self.__str__())

class Qtrn():
    def __init__(self, w=0,x=0,y=0,z=0) -> Self:
        self.w=w
        self.x=x
        self.y=y
        self.z=z        

    def copy(self) -> Self:
        return Qtrn(self.w, self.x, self.y, self.z)
        
    def mag(self) -> float:
        return math.sqrt(self.w**2 +self.x**2 +self.y**2 +self.z**2)

    def conj(self) -> Self:
        return Qtrn(self.w, -self.x, -self.y, -self.z)
    
    def multiply( self, q2 ) -> Self:
        """Quaternion multiplication"""
        q1 = self
        w = q1.w * q2.w - q1.x * q2.x - q1.y * q2.y - q1.z * q2.z
        x = q1.w * q2.x + q1.x * q2.w + q1.y * q2.z - q1.z * q2.y
        y = q1.w * q2.y - q1.x * q2.z + q1.y * q2.w + q1.z * q2.x
        z = q1.w * q2.z + q1.x * q2.y - q1.y * q2.x + q1.z * q2.w
        self.w, self.x, self.y, self.z  = (w,x,y,z)
        return self

    def normalize(self) -> Self:
        """Normalize quaternion"""
        mag = self.mag()
        if mag > 0.0:
            self.w /= mag
            self.x /= mag
            self.y /= mag
            self.z /= mag
        else:
            printf("Error - normalize_quat() - something wrong with quaternion - divide by zero, mag = 0.0 \n")
        return self

    def getEuler(self) -> Vec_pqr: 
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
        cosy_cosp = 1.0 - 2.0 * ( self.y**2 + self.z**2 )
        yaw = math.atan2( siny_cosp, cosy_cosp )
        return Vec_pqr(p=roll, q=pitch, r=yaw)

    def Heading(self) -> float: 
        """Compute Heading Ang_le ( radians )"""
        ## Rotate body x-axis ( 1,0,0 ) Int_o inrt frame
        fx = 1 - 2*(self.y**2 +self.z**2)
        fz = 2 * ( self.x*self.z - self.w*self.y )
        return math.atan2( fx, fz ) #< atan2( east, north )

    def __str__(self) -> str:
        s = ""
        for v in (self.w, self.x, self.y, self.z):
            s += "%1.3f, "%(v)
        return s[0:-2]

    def print(self) -> None:
        print("Qtrn_wxyz:, " +self.__str__())


def body_to_earth_M( body, q ) -> Vec_xyz: 
    """Rotate vector by a quaternion using matrix math"""
    x = q.x
    y = q.y
    z = q.z
    w = q.w

    R11 = 1 - 2 * ( y * y + z * z )
    R12 = 2     * ( x * y - z * w )
    R13 = 2     * ( x * z + y * w )

    R21 = 2     * ( x * y + z * w )
    R22 = 1 - 2 * ( x * x + z * z )
    R23 = 2     * ( y * z - x * w )

    R31 = 2     * ( x * z - y * w )
    R32 = 2     * ( y * z + x * w )
    R33 = 1 - 2 * ( x * x + y * y )

    return Vec_xyz(
        R11 * body.x + R12 * body.y + R13 * body.z,
        R21 * body.x + R22 * body.y + R23 * body.z,
        R31 * body.x + R32 * body.y + R33 * body.z)

def body_to_earth_Q( body, q ) -> Vec_xyz:
    """Rotate vector by a quaternion using Quaternion math"""
    p = Qtrn(w=0, x=body.x, y=body.y, z=body.z)
    q = q.copy()
    q_conj = q.conj()
    q.multiply( p ).multiply( q_conj )
    return Vec_xyz(x=q.x, y=q.y, z=q.z)
 
##================================================================================================================= 

if __name__ == "__main__":
    print("Still missing unit-tests")
    Vec_xyz(1.1, 2.2, 3.3).print()
    Vec_pqr(1.1, 2.2, 3.3).print()
    Qtrn(1.1, 2.2, 3.3, 4.4).print()
    
