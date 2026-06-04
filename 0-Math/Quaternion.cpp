//345678901234567890123456789012345678901234567890123456789012345678901234567890
//==============================================================================
//
//       MATH Function,  Quaternion.cpp
//						Two quaternion libraries were merged,  need to unify more
//
//==============================================================================


#include "Quaternion.h"


//=======================================================================================================
/* Default initialization (Identity quaternion ) */
void Quaternion_Init(Quaternion4d * q )
{
    if( q != NULL )
    {
        q->x = 0.0;
        q->y = 0.0;
        q->z = 0.0;
        q->w = 1.0;
    }
}

/* Initialization with values */
void Quaternion_InitXYZW(Quaternion4d * q, double x, double y, double z, double w )
{
    if( q != NULL )
    {
        q->x = x;
        q->y = y;
        q->z = z;
        q->w = w;
    }
}


//=======================================================================================================
/* Copy */
void Quaternion_Copy(Quaternion4d * dest, const Quaternion4d * src )
{
    if( dest != NULL && src != NULL )
    {
       * dest = * src;
    }
}


//=======================================================================================================
/* Create quaternion from Yaw, Pitch, Roll (in radians ) */
Quaternion4d Quaternion_RotationYawPitchRoll(float yaw, float pitch, float roll )
{
    Quaternion4d q;
    float fSinPitch = (float )sin(pitch * 0.5f );
    float fCosPitch = (float )cos(pitch * 0.5f );
    float fSinYaw   = (float )sin(yaw   * 0.5f );
    float fCosYaw   = (float )cos(yaw   * 0.5f );
    float fSinRoll  = (float )sin(roll  * 0.5f );
    float fCosRoll  = (float )cos(roll  * 0.5f );

    float fCosPitchCosYaw = fCosPitch * fCosYaw;
    float fSinPitchSinYaw = fSinPitch * fSinYaw;

    q.x = fSinRoll * fCosPitchCosYaw - fCosRoll * fSinPitchSinYaw;
    q.y = fCosRoll * fSinPitch 		 * fCosYaw  + fSinRoll 		 * fCosPitch * fSinYaw;
    q.z = fCosRoll * fCosPitch 		 * fSinYaw  - fSinRoll 		 * fSinPitch * fCosYaw;
    q.w = fCosRoll * fCosPitchCosYaw + fSinRoll * fSinPitchSinYaw;

    return q;
}


//=======================================================================================================
/* Multiply */

Quaternion4d quaternion_multiply( Quaternion4d a, Quaternion4d b  ) 
{
  return( Quaternion4d  )
                     {   a.w * b.w - a.x * b.x - a.y * b.y - a.z * b.z,
                         a.w * b.x + a.x * b.w + a.y * b.z - a.z * b.y,
                         a.w * b.y - a.x * b.z + a.y * b.w + a.z * b.x,
                         a.w * b.z + a.x * b.y - a.y * b.x + a.z * b.w    
                     };
}


//=======================================================================================================
/* Convert Quaternion to 4x4 Transform Matrix */
struct Matrix Quaternion_ToTransformMatrix(const Quaternion4d * q )
{
    struct Matrix m = * Matrix_Create4x4( );
    Matrix_InitIdentity(&m );

    if( q == NULL )
        return m;

    double xx = q->x * q->x;
    double yy = q->y * q->y;
    double zz = q->z * q->z;
    double xy = q->x * q->y;
    double xz = q->x * q->z;
    double yz = q->y * q->z;
    double wx = q->w * q->x;
    double wy = q->w * q->y;
    double wz = q->w * q->z;

    Matrix_Set(&m, 0, 0, 1.0 - 2.0 * (yy + zz ) );
    Matrix_Set(&m, 0, 1, 2.0 * (xy - wz ) );
    Matrix_Set(&m, 0, 2, 2.0 * (wy + xz ) );

    Matrix_Set(&m, 1, 0, 2.0 * (xy + wz ) );
    Matrix_Set(&m, 1, 1, 1.0 - 2.0 * (xx + zz ) );
    Matrix_Set(&m, 1, 2, 2.0 * (yz - wx ) );

    Matrix_Set(&m, 2, 0, 2.0 * (xz - wy ) );
    Matrix_Set(&m, 2, 1, 2.0 * (yz + wx ) );
    Matrix_Set(&m, 2, 2, 1.0 - 2.0 * (xx + yy ) );

    return m;
}


//=======================================================================================================

 Quaternion4d quaternion_rotate_vector( Quaternion4d q,  Vector3d v  ) 
{
    Quaternion4d vq;
  
    vq = ( Quaternion4d  ){  0,  v.x,  v.y,  v.z };
    vq = quaternion_multiply( q, vq   );
    vq = quaternion_multiply( vq, quaternion_conjugate( q  ));

//	vq = quaternion_multiply( vq, Quaternion_Conjugate( q  ));
    return(  Quaternion4d  ){  vq.x,  vq.y,  vq.z };
}


//=======================================================================================================
/* Conjugate */
Quaternion4d Quaternion_Conjugate(const Quaternion4d * q )
{
    Quaternion4d result;
    Quaternion_Init(&result );

    if( q != NULL )
    {
        result.w =  q->w;   
        result.x = -q->x;
        result.y = -q->y;
        result.z = -q->z;
    }
    return result;
}


Quaternion4d quaternion_conjugate( Quaternion4d q  ) 
{
  return( Quaternion4d  ){ q.w, -q.x, -q.y, -q.z };
}


//=======================================================================================================
/* Invert */
Quaternion4d Quaternion_Invert(const Quaternion4d * q )
{
    Quaternion4d result;
    Quaternion_Init(&result );

    if( q != NULL )
    {
        double lengthSq = q->x * q->x + q->y * q->y + q->z * q->z + q->w * q->w;
        if( lengthSq > EPSILON )
        {
            double inv = 1.0 / lengthSq;
            result.x = -q->x * inv;
            result.y = -q->y * inv;
            result.z = -q->z * inv;
            result.w =  q->w * inv;
        }
    }
    return result;
}


//=======================================================================================================
/* Normalize */
void Quaternion_Normalize(Quaternion4d * q )
{
    if( q == NULL )
        return;

    double length = sqrt(q->x * q->x + q->y * q->y + q->z * q->z + q->w * q->w );

    if( length > EPSILON )
    {
        q->x /= length;
        q->y /= length;
        q->z /= length;
        q->w /= length;
    }
    else
    {
        q->x = 0.0;
        q->y = 0.0;
        q->z = 0.0;
        q->w = 1.0;
    }
}

Quaternion4d quaternion_normalize( Quaternion4d q   ) 
{
    double n;
    n = sqrt( q.x * q.x + q.y * q.y + q.z * q.z + q.w * q.w  );
    return( Quaternion4d  ){ (float )(q.w/n ), (float )(q.x/n ), (float )(q.y/n ), (float )(q.z/n ) };
}


//=======================================================================================================
/* Convert to Euler Angles (in radians ) */

Vector3 Quaternion_ToEulerAngles( const Quaternion4d * q  )
{
    Vector3 result = {0.0f, 0.0f, 0.0f};
    
    if( q == NULL )
        return result;

    Quaternion4d qn = * q;
    Quaternion_Normalize(&qn );

    // Roll (X )
    double sinr_cosp = 2.0 * (qn.w * qn.x + qn.y * qn.z );
    double cosr_cosp = 1.0 - 2.0 * (qn.x * qn.x + qn.y * qn.y );
    result.x = (float )atan2(sinr_cosp, cosr_cosp );

    // Pitch (Y )
    double sinp = 2.0 * (qn.w * qn.y - qn.z * qn.x );
    if( fabs(sinp ) >= 1.0 )
        result.y = (float )(sinp > 0.0 ? M_PI/2.0 : -M_PI/2.0 );
    else
        result.y = (float )asin(sinp );

    // Yaw (Z )
    double siny_cosp = 2.0 * (qn.w * qn.z + qn.x * qn.y );
    double cosy_cosp = 1.0 - 2.0 * (qn.y * qn.y + qn.z * qn.z );
    result.z = (float )atan2(siny_cosp, cosy_cosp );

    return result;
}

void quaternion_to_euler( Quaternion4d q, double * pitch, double * yaw, double * roll  ) 
{
	// Function to convert a quaternion to Euler angles (degrees )
    // Normalize the quaternion
    
    double norm = sqrt(q.w * q.w + q.x * q.x + q.y * q.y + q.z * q.z );
    q.w /= norm;
    q.x /= norm;
    q.y /= norm;
    q.z /= norm;

    // Compute Euler angles
    // Roll
    double sinr_cosp = 2 * ( q.w * q.x + q.y * q.z  );
    double cosr_cosp = 1 - 2 * ( q.x * q.x + q.y * q.y  );
   * roll = RAD_TO_DEG( atan2( sinr_cosp, cosr_cosp  ));

	// Pitch
    double sinp = 2 * ( q.w * q.y - q.z * q.x  );
    if( fabs( sinp  ) >= 1  ) * pitch = RAD_TO_DEG( copysign(M_PI / 2, sinp  )); // Use 90 degrees if out of range
    else                      * pitch = RAD_TO_DEG( asin( sinp  ) );

	// Yaw
    double siny_cosp = 2 * ( q.w * q.z + q.x * q.y  );
    double cosy_cosp = 1 - 2 * ( q.y * q.y + q.z * q.z  );
   * yaw = RAD_TO_DEG( atan2( siny_cosp, cosy_cosp  ) );
}


//=======================================================================================================
// Function to convert Euler angles (degrees ) to a quaternion

Quaternion4d euler_to_quaternion( double pitch, double yaw, double roll  ) 
{
    // Convert degrees to radians
    pitch = DEG_TO_RAD(pitch );
    yaw   = DEG_TO_RAD(yaw   );
    roll  = DEG_TO_RAD(roll  );

    double cy = cos(   yaw * 0.5  );
    double sy = sin(   yaw * 0.5  );
    double cp = cos( pitch * 0.5  );
    double sp = sin( pitch * 0.5  );
    double cr = cos(  roll * 0.5  );
    double sr = sin(  roll * 0.5  );

    Quaternion4d q;
    q.w = cr * cp * cy + sr * sp * sy;
    q.x = sr * cp * cy - cr * sp * sy;
    q.y = cr * sp * cy + sr * cp * sy;
    q.z = cr * cp * sy - sr * sp * cy;

    return q;
}


//====================================================================================

Quaternion4d make_rotation_quaternion_from_axis_and_angle(  Quaternion4d axis, float angle   ) 
{
   Quaternion4d q;
   q = ( Quaternion4d  ){ cos( angle/2.0  ),  axis.x * sin( angle/2.0  ), 
                axis.y * sin( angle/2.0  ),  axis.z * sin( angle/2.0  )    };
   return q; 
}


Quaternion4d Quat_to_Axis( Quaternion4d q1  ) 
{
   float a, x2, y2, z2, heading, bank, attitude;

    double sqw = q1.w * q1.w;
    double sqx = q1.x * q1.x;
    double sqy = q1.y * q1.y;
    double sqz = q1.z * q1.z;

    double unit = sqx + sqy + sqz + sqw; // if normalised is one, otherwise is correction factor
    double test = q1.x * q1.y + q1.z * q1.w;

    a = 1.0;

	if( test > 0.499 * unit  ) 
    {                                                // singularity at north pole
	    heading  = 2 * atan2( q1.x, q1.w  );
	    attitude = 3.1415 / 2.0;
	    bank     = 0;
	}
	else if( test < -0.499 * unit  ) 
    {                                                // singularity at south pole
	    heading  = -2 * atan2( q1.x, q1.w  );
	    attitude = -3.1415 / 2.0;
	    bank     = 0;
	}
    else
    {
        heading  = atan2( ( 2.0 * q1.y * q1.w  ) - ( 2.0 * q1.x * q1.z  ) ,  sqx - sqy - sqz + sqw  );
	    attitude = asin(    2.0 * test/unit    );
	    bank     = atan2( ( 2.0 * q1.x * q1.w  ) - ( 2.0 * q1.y * q1.z  ) , -sqx + sqy - sqz + sqw  );
    } 

   x2 =  (float )( bank     / (2 * 3.1415 ) * 360.0  );
   y2 =  (float )( heading  / (2 * 3.1415 ) * 360.0  );
   z2 =  (float )( attitude / (2 * 3.1415 ) * 360.0  );

   return( Quaternion4d  ){ a, x2, y2, z2 };
}


//====================================================================================


























