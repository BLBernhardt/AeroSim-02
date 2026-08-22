//345678901234567890123456789012345678901234567890123456789012345678901234567890
//==============================================================================
//
//       MATH Functions, Quaternion
//
//==============================================================================

//#include "Math.h"

#if 0
float vector_dot_product(  Qvector3f a,  Qvector3f b ) 
{
  return a.x*b.x + a.y*b.y + a.z*b.z;
}

float vector_norm(  Qvector3f v ) 
{
  return sqrt( v.x*v.x + v.y*v.y + v.z*v.z );
}

 Qvector3f vector_add(  Qvector3f a,  Qvector3f b  ) 
{
  return (  Qvector3f ){ a.x + b.x, a.y + b.y, a.z + b.z };
}

 Qvector3f vector_scale( float constant,  Qvector3f v ) 
{
  return (  Qvector3f ){ constant*v.x, constant*v.y, constant*v.z };
}

 Qvector3f vector_normalize(  Qvector3f v ) 
{
  return vector_scale( 1/vector_norm( v ), v );
}


#endif
#if 0 
 Qvector3f vector_cross_product(  Qvector3f a,  Qvector3f b  ) 
{
  return (  Qvector3f ){ a.y*b.z - a.z*b.y, a.x*b.z - a.z*b.x, a.x*b.y - a.y*b.x };
}

Quaternion make_rotation_quaternion_from_axis_and_angle(  Qvector3f axis, float angle  ) 
{
   Quaternion q;
   q = ( Quaternion ){ cos( angle/2.0 ),           axis.x * sin( angle/2.0 ), 
                       axis.y * sin( angle/2.0 ),  axis.z *sin( angle/2.0 )    };
   return q; 
}

Quaternion quaternion_multiply( Quaternion a, Quaternion b ) 
{
  return ( Quaternion )
                     {   a.w * b.w - a.x * b.x - a.y * b.y - a.z * b.z,
                         a.w * b.x + a.x * b.w + a.y * b.z - a.z * b.y,
                         a.w * b.y - a.x * b.z + a.y * b.w + a.z * b.x,
                         a.w * b.z + a.x * b.y - a.y * b.x + a.z * b.w    
                     };
}

Quaternion quaternion_conjugate( Quaternion q ) 
{
  return ( Quaternion ){ q.w, -q.x, -q.y, -q.z };
}

 Qvector3f quaternion_rotate_vector( Quaternion q,  Qvector3f v  ) 
{
    Quaternion vq;
  
    vq = ( Quaternion ){  0,  v.x,  v.y,  v.z };
    vq = quaternion_multiply( q, vq  );
    vq = quaternion_multiply( vq, quaternion_conjugate( q )  );
    return (  Qvector3f ){  vq.x,  vq.y,  vq.z };
}

Quaternion quaternion_normalize( Quaternion q  ) 
{
    double n;
    n = sqrt( q.x*q.x + q.y*q.y + q.z*q.z + q.w*q.w );
    return  ( Quaternion ){ (float)(q.w/n), (float)(q.x/n), (float)(q.y/n), (float)(q.z/n) };
}

Qvector4f Quat_to_Axis( Quaternion q1 ) 
{
   float a, x2, y2, z2, heading, bank, attitude;

    double sqw = q1.w*q1.w;
    double sqx = q1.x*q1.x;
    double sqy = q1.y*q1.y;
    double sqz = q1.z*q1.z;

    double unit = sqx + sqy + sqz + sqw; // if normalised is one, otherwise is correction factor
    double test = q1.x*q1.y + q1.z*q1.w;

    a = 1.0;

	if (test > 0.499*unit) 
    {                                                // singularity at north pole
	    heading  = 2 * atan2( q1.x, q1.w );
	    attitude = 3.1415 / 2.0;
	    bank     = 0;
	}
	else if (test < -0.499*unit ) 
    {                                                // singularity at south pole
	    heading  = -2 * atan2( q1.x, q1.w );
	    attitude = -3.1415 / 2.0;
	    bank     = 0;
	}
    else
    {
        heading  = atan2( ( 2.0 * q1.y * q1.w ) - ( 2.0 * q1.x * q1.z ) ,  sqx - sqy - sqz + sqw );
	    attitude = asin(    2.0 * test/unit   );
	    bank     = atan2( ( 2.0 * q1.x * q1.w ) - ( 2.0 * q1.y * q1.z ) , -sqx + sqy - sqz + sqw );
    } 

   x2 =  (float)( bank     / (2*3.1415) * 360.0 );
   y2 =  (float)( heading  / (2*3.1415) * 360.0 );
   z2 =  (float)( attitude / (2*3.1415) * 360.0 );

   return ( Qvector4f ){ a, x2, y2, z2 };
}

Quaternion euler_to_quaternion( double pitch, double yaw, double roll ) 
{

	// Function to convert Euler angles (degrees) to a quaternion
    // Convert degrees to radians
    
    pitch = DEG_TO_RAD(pitch);
    yaw   = DEG_TO_RAD(yaw);
    roll  = DEG_TO_RAD(roll);

    double cy = cos(   yaw * 0.5 );
    double sy = sin(   yaw * 0.5 );
    double cp = cos( pitch * 0.5 );
    double sp = sin( pitch * 0.5 );
    double cr = cos(  roll * 0.5 );
    double sr = sin(  roll * 0.5 );

    Quaternion q;
    q.w = cr * cp * cy + sr * sp * sy;
    q.x = sr * cp * cy - cr * sp * sy;
    q.y = cr * sp * cy + sr * cp * sy;
    q.z = cr * cp * sy - sr * sp * cy;

    return q;
}

void quaternion_to_euler( Quaternion q, double *pitch, double *yaw, double *roll ) 
{
	// Function to convert a quaternion to Euler angles (degrees)
    // Normalize the quaternion
    
    double norm = sqrt(q.w * q.w + q.x * q.x + q.y * q.y + q.z * q.z);
    q.w /= norm;
    q.x /= norm;
    q.y /= norm;
    q.z /= norm;

    // Compute Euler angles
    // Roll
    double sinr_cosp = 2 * ( q.w * q.x + q.y * q.z );
    double cosr_cosp = 1 - 2 * ( q.x * q.x + q.y * q.y );
    *roll = RAD_TO_DEG( atan2( sinr_cosp, cosr_cosp ));

	// Pitch
    double sinp = 2 * ( q.w * q.y - q.z * q.x );
    if( fabs( sinp ) >= 1 ) *pitch = RAD_TO_DEG( copysign(M_PI / 2, sinp )); // Use 90 degrees if out of range
    else                    *pitch = RAD_TO_DEG( asin( sinp ));

	// Yaw
    double siny_cosp = 2 * ( q.w * q.z + q.x * q.y );
    double cosy_cosp = 1 - 2 * ( q.y * q.y + q.z * q.z );
    *yaw = RAD_TO_DEG( atan2( siny_cosp, cosy_cosp ));
}

Quaternion multiply_quaternions( Quaternion q1, Quaternion q2 ) 
{
    Quaternion result;
    
    result.w = q1.w * q2.w - q1.x * q2.x - q1.y * q2.y - q1.z * q2.z;
    result.x = q1.w * q2.x + q1.x * q2.w + q1.y * q2.z - q1.z * q2.y;
    result.y = q1.w * q2.y - q1.x * q2.z + q1.y * q2.w + q1.z * q2.x;
    result.z = q1.w * q2.z + q1.x * q2.y - q1.y * q2.x + q1.z * q2.w;
    return result;
}
#endif

