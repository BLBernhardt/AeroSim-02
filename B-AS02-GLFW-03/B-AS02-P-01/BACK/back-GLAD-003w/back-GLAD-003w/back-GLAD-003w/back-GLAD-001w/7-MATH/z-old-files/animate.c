

void animate_2( float dt ) 
{
  vector plane_movement;
  vector rotation_axis;
  quaternion rotation;
  float speed;

  vector x,   y,   z;
  vector x_t, y_t, z_t;
  
  if( dt < 0.0001 ) return;
  if( mouse_x * mouse_x + mouse_y * mouse_y < 0.0001  ) return;

  plane_movement = vector_scale( dt*3, make_vector( mouse_x, mouse_y, 0 ));
  plane_position = vector_add( plane_position, plane_movement  );
  
  speed           = vector_norm( plane_movement );

  plane_movement.x = -plane_movement.x; // Why do I have this line here? :/
  														// check and see if its related to the odd opengl Z axis

  rotation_axis   = vector_normalize( vector_cross_product( downwards, plane_movement ) );


  x = make_vector( 1, 0, 0 );
  y = make_vector( 0, 1, 0 );
  z = make_vector( 0, 0, 1 );

  cnt++;
  if( cnt < 100 )
  {
     plane_movement = ( vector ){ 0.707, 0.707, 0.707 };
     rotation_axis   = vector_normalize( plane_movement );
     speed = 0.01;
  }
  if( cnt >= 100 &&  cnt < 300 )
  {
     plane_movement = ( vector ){ 0.3931, 0.87846, -0.2715 };
     rotation_axis   = vector_normalize( plane_movement );
     speed = 0.01;
  }

  if( cnt >= 300 &&  cnt < 500 )
  {
     plane_movement = ( vector ){ -0.434480, 0.42928,  0.791797  };
     rotation_axis   = vector_normalize( plane_movement );
     speed = 0.01;
  }

  if( cnt >= 500 &&  cnt < 600 )
  {
     speed = 0.006;
     x_t = quaternion_rotate_vector(  plane_orientation_q, x );
     y_t = quaternion_rotate_vector(  plane_orientation_q, y );
     z_t = quaternion_rotate_vector(  plane_orientation_q, z );


//     plane_movement = ( vector ){ -0.734015 ,0.340700 ,-0.587486  };
     plane_movement = ( vector ){ z_t.x, z_t.y, z_t.z };
     rotation_axis   = vector_normalize( plane_movement );
 
  }
  if( cnt >= 600  ) 
  {
     speed = 0.001;
     x_t = quaternion_rotate_vector(  plane_orientation_q, x );
     y_t = quaternion_rotate_vector(  plane_orientation_q, y );
     z_t = quaternion_rotate_vector(  plane_orientation_q, z );
     plane_movement = ( vector ){ x_t.x, x_t.y, x_t.z };

     rotation_axis   = vector_normalize( plane_movement );
  }



  printf(" x %f y %f z %f \n",   x_t.x, x_t.y, x_t.z );
  printf(" x %f y %f z %f \n",   y_t.x, y_t.y, y_t.z );
  printf(" x %f y %f z %f \n\n", z_t.x, z_t.y, z_t.z );




/* >>>>>>>>> This is it!     <<<<<<<<<<*/

  rotation = make_rotation_quaternion_from_axis_and_angle( rotation_axis, speed * 1.481  );
  plane_orientation_q = quaternion_multiply( rotation, plane_orientation_q  );

/* >>>>>>>>> This is it!     <<<<<<<<<<*/


}

