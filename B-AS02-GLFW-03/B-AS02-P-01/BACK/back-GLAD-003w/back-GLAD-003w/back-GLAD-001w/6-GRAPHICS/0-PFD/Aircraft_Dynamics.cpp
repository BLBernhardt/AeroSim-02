

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include "../structures.h"
#include "defines.h"


 extern struct _Aerodyn    Aero;
 extern struct _Discrete   Disc;
 extern struct _Frame      Frame;
 extern struct _Cntrls     Cntrls;
 extern struct _Display    Display;

void AircraftDynamics( int mode )
{

  if( mode == INIT )
  {
    Aero.Heading_deg_r       = 0.01;
    Aero.Altitude_AGL_feet_r = 0.3;
//    Aero.AirSpeed_knots_r    = 0.1;
    Aero.Roll_deg_r          = 0.003;
    Aero.Pitch_deg_r         = 0.003;

    Aero.Heading_deg         = 330.0;

    Aero.Roll_deg            =  2.0;
    Aero.Pitch_deg           =  8.0; 
    Aero.AirSpeed_meters_ps  = 123.0; 

    Aero.Altitude_AGL_meters = 15000.0/3.2808;    // 3.2808 ft per meter

  } 
  if( mode == RUN   )
  {
//     Aero.AirSpeed_knots    += Aero.AirSpeed_knots_r;
     Aero.Pitch_deg         += Aero.Pitch_deg_r;  
     Aero.Roll_deg          += Aero.Roll_deg_r;	   
     Aero.Heading_deg       += Aero.Heading_deg_r; 
     Aero.Altitude_AGL_feet += Aero.Altitude_AGL_feet_r; 

 //    if( Aero.AirSpeed_knots    <    10 ) Aero.AirSpeed_knots    =    10;
     if( Aero.Pitch_deg         <     0 ) Aero.Pitch_deg         =   360;
     if( Aero.Roll_deg          <     0 ) Aero.Roll_deg          =   360;  
     if( Aero.Heading_deg       <     0 ) Aero.Heading_deg       =   360;
     if( Aero.Altitude_AGL_feet <     0 ) Aero.Altitude_AGL_feet =     0;

 //    if( Aero.AirSpeed_knots    >   600 ) Aero.AirSpeed_knots    =   600;
     if( Aero.Pitch_deg         >   360 ) Aero.Pitch_deg         =     0;
     if( Aero.Roll_deg          >   360 ) Aero.Roll_deg          =     0;  
     if( Aero.Heading_deg       >   360 ) Aero.Heading_deg       =     0;
     if( Aero.Altitude_AGL_feet > 90000 ) Aero.Altitude_AGL_feet = 90000;
     if( 0 )
     {

        printf(" Head %5.2f Alt   %5.2f  Roll %5.2f Pitch %5.2f\n", 
	        Aero.Heading_deg_r,
	        Aero.Altitude_AGL_feet_r,
	        Aero.Roll_deg_r,
	        Aero.Pitch_deg_r );
     }

  }
}



 
