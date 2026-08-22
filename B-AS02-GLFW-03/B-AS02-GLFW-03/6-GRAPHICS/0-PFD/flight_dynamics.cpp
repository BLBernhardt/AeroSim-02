#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <GL/glut.h>
#include "../structures.h"
#include "defines.h"



extern struct _Cntrls Cntrls;
extern struct _Aerodyn Aero;
extern struct _Frame Frame;
extern struct _Discrete Disc;

void Dynamics_Init()
{


 Aero.Heading_deg_r = 0.01;
 Aero.Altitude_AGL_feet_r = 0.3;
 Aero.AirSpeed_knots_r = 0.1;
 Aero.Roll_deg_r = 0.3;
 Aero.Pitch_deg_r = 0.3;

 Aero.Heading_deg = 330.0;
 Aero.Altitude_AGL_meters = 10000.0;
 Aero.Roll_deg = -2.0;
 Aero.Pitch_deg = 5.0; 
	Aero.AirSpeed_knots_max = 700.0;
 Aero.AirSpeed_knots = Aero.AirSpeed_knots_max/2.0;
 

} 

#if 0
void Dynamics_Run()
{

 Aero.AirSpeed_knots += Aero.AirSpeed_knots_r;
 Aero.Pitch_deg += Aero.Pitch_deg_r; 
 Aero.Roll_deg += Aero.Roll_deg_r;	 
 Aero.Heading_deg += Aero.Heading_deg_r; 
 Aero.Altitude_AGL_feet += Aero.Altitude_AGL_feet_r; 

 if( Aero.AirSpeed_knots < 10 ) Aero.AirSpeed_knots = 10;
 if( Aero.Pitch_deg < 0 ) Aero.Pitch_deg = 360;
 if( Aero.Roll_deg < 0 ) Aero.Roll_deg = 360; 
 if( Aero.Heading_deg < 0 ) Aero.Heading_deg = 360;
 if( Aero.Altitude_AGL_feet < 0 ) Aero.Altitude_AGL_feet = 0;

 if( Aero.AirSpeed_knots > 600 ) Aero.AirSpeed_knots = Aero.AirSpeed_knots_max;
 if( Aero.Pitch_deg > 360 ) Aero.Pitch_deg = 0;
 if( Aero.Roll_deg > 360 ) Aero.Roll_deg = 0; 
 if( Aero.Heading_deg > 360 ) Aero.Heading_deg = 0;
 if( Aero.Altitude_AGL_feet > 90000 ) Aero.Altitude_AGL_feet = 90000;


 if( 0 )
 {
 printf(" 3:4-Head %5.2f 1:2-Alt %5.2f 9:0-Speed %5.2f 5:6-Roll %5.2f 7:8-Pitch %5.2f\n", 
	 Aero.Heading_deg_r,
	 Aero.Altitude_AGL_feet_r,
	 Aero.AirSpeed_knots_r,
	 Aero.Roll_deg_r,
	 Aero.Pitch_deg_r );
 }
}
#endif


void
flight_dynamics( void )
{
 static float movex = 0.0, movey = 0.0; /* Scaled mouse movement 	*/
 GLfloat distance; /* Distance to move 	*/
 GLfloat cheading; /* Cosine of heading 	*/
 GLfloat sheading; /* Sine of heading 	*/
 static float cpitch; /* Cosine of pitch 	*/
 static float spitch; /* Sine of pitch 		*/
 GLfloat Orientation[3];
 GLfloat Position[3];
 GLfloat Velocity;
 // GLfloat dlt;
 float speed_cmd;
 

 // dlt = (float)Frame.Frames_per_second;

 	distance = 0.01;


 	// if( Cntrls.Button_Down == 1 )
 	// {
 	// movex = Cntrls.MouseX/1.0;
 	// movey = Cntrls.MouseY/1.0;
 
 	movex = Cntrls.aileron_cmd_norm;
 	movey = Cntrls.elevator_cmd_norm;
 
 
 	//printf(" %f %f \n", movex, movey );
 	// }
 	if( movex > +2.0 ) movex = movex - 0.1;
 	if( movex < -2.0 ) movex = movex + 0.1;
 	if( movey > +2.0 ) movey = movey - 0.1;
 	if( movey < -2.0 ) movey = movey + 0.1;


 	//Aero.AirSpeed_knots_max = 700.0;
 	//Aero.AirSpeed_knots_vert_climb_max = 120.0;

 	Position[0] = Aero.Position_X_meters;
 	Position[1] = Aero.Altitude_AGL_meters;
 	Position[2] = Aero.Position_Z_meters;
 	Orientation[0] = Aero.Pitch_deg;
 	Orientation[1] = Aero.Yaw_deg;
 	Orientation[2] = Aero.Roll_deg;

 	// 	Aero.AirSpeed_meters_ps = Aero.AirSpeed_meters_ps - ( Aero.AirSpeed_accl_meters_pss/dlt );
 	// 	printf(" FR %f \n" dlt );
 	// 	printf(" Accl %f \n", Aero.AirSpeed_accl_meters_pss );
 	// 	printf(" accl pss %f \n", Aero.AirSpeed_accl_meters_pss/dlt );
 	// Aero.AirSpeed_meters_ps = 100.0;
 	//	if( Aero.AirSpeed_meters_ps < 1 ) Aero.AirSpeed_meters_ps = 1;
 	// 	Aero.AirSpeed_knots = Aero.AirSpeed_meters_ps * 1.94384;

 	speed_cmd = Aero.AirSpeed_knots_max/2.0 * (1.0 - spitch);
 	Aero.AirSpeed_knots = Aero.AirSpeed_knots + (speed_cmd - Aero.AirSpeed_knots) * 0.0003;

 	// printf(" %f %f %f \n", Aero.AirSpeed_knots, speed_cmd, (Aero.AirSpeed_knots - speed_cmd) * 0.0001 );

 	Velocity = Aero.AirSpeed_knots/200.0;

 	if( Position[1] <= 2.0 && Velocity <= 2.0 )
 	{
 		// movex = 0.0;
 		// movey = 0.0;
 	}

 	// Orientation[0] 	Pitch 
 	// Orientation[1] 	Yaw 
 	// Orientation[2] 	Roll 
 	// Position[0] 	X 
 	// Position[1] 	Z 
 	// Position[2] 	Y 

 	// Orientation[0] += distance * movey * cos(Orientation[2] * M_PI / 180.0);
 	Orientation[0] += 0.3 * distance * movey * cos(Orientation[2] * M_PI / 180.0);
 	// Orientation[1] -= 0.2* distance * movey * sin(Orientation[2] * M_PI / 180.0);
 	Orientation[2] += distance * movex;

 	if( movey > 0 ) 
 	{
 		Orientation[1] -= 2.0 * distance * movey * sin(Orientation[2] * M_PI / 180.0);
 	}

 	if (Orientation[0] < -90.0)
 	{
 		Orientation[0] = -180.0 - Orientation[0];
 		Orientation[2] += 180.0;
 	}
 	else if (Orientation[0] > 90.0)
 	{
 		Orientation[0] = 180.0 - Orientation[0];
 		Orientation[2] -= 180.0;
 	}

 	if (Orientation[1] < 0.0) Orientation[1] += 360.0;
 	else if (Orientation[1] > 360.0) Orientation[1] -= 360.0;

 	if (Orientation[2] < -180.0) Orientation[2] += 360.0;
 	else if (Orientation[2] > 180.0) Orientation[2] -= 360.0;

 	if( Position[1] <= 0.0 ) Position[1] = 0;

/*-----------------------------------------------------------------------------*/
/* Move based upon the current orientation... */

 	cheading = cos(Orientation[1] * M_PI / 180.0);
 	sheading = sin(Orientation[1] * M_PI / 180.0);
 	cpitch = cos(Orientation[0] * M_PI / 180.0);
 	spitch = sin(Orientation[0] * M_PI / 180.0);

 	Position[0] += Velocity * sheading * cpitch;	// 
 	Position[2] -= Velocity * cheading * cpitch;
 	Position[1] += Velocity * spitch * 1.20;		// Altitude
 
 	Aero.Altitude_AGL_meters = Position[1]; /* Altitude in Meters */
 	/* 3.2808 feet per meter */
 	Aero.Altitude_Radar_feet = (Position[1] * 3.2808) + Aero.Flight_Deck_Hieght_feet;

 	Disc.WOW = 0;
 	//	printf(">>F %f \n", Position[1] );
	 // if( Position[1] <= .1 && Aero.Airspeed_feet_per_sec <= 60 ) 
 
 	// if( Position[1] <= .1 ) 
 	if( 0 )
 	{
 		Position[1] = 0.1;
		printf(">>On Ground\n");

 		if( Orientation[2] > 0.0) Orientation[2] = Orientation[2] - 1; 
 		if( Orientation[2] < 0.0) Orientation[2] = Orientation[2] + 1;
 
 		if( Orientation[0] > 0.0) Orientation[0] = Orientation[0] - .01;
 		if( Orientation[0] < 0.0) Orientation[0] = 0.0 ; 

 		//if ( ground_turn_L == 1) Orientation[1] = Orientation[1] - 1.0; 
 		//if ( ground_turn_R == 1) Orientation[1] = Orientation[1] + 1.0; 

 		//ground_turn_R = 0;
 		//ground_turn_L = 0;
 		Disc.WOW = 1;
 	}
 
 	float restore_rate = 0.05;
 
 	if( !Cntrls.Stick_Enable )
 	{ 
 		if( Orientation[0] > 0.0 ) Orientation[0] -= restore_rate;
 		if( Orientation[0] < 0.0 ) Orientation[0] += restore_rate; 
 		//if( Orientation[1] > 0.0 ) Orientation[1] -= restore_rate;
 		//if( Orientation[1] < 0.0 ) Orientation[1] += restore_rate; 
 		if( Orientation[2] > 0.0 ) Orientation[2] -= restore_rate;
 		if( Orientation[2] < 0.0 ) Orientation[2] += restore_rate; 
 	}
 
 	Aero.Position_X_meters = Position[0];
 	Aero.Altitude_AGL_meters = Position[1];
 	Aero.Position_Z_meters = Position[2];
 	Aero.Pitch_deg = Orientation[0];
 	Aero.Yaw_deg = Orientation[1];
 	Aero.Roll_deg = Orientation[2];
 
 	Aero.Altitude_AGL_feet = Aero.Altitude_AGL_meters * 3.2808;
 	Aero.Heading_deg = Orientation[1];
 
 // Position[1] = Position[1] - 0.9;
 // printf(" position x:%f y:%f z:%f \n", Position[0], Position[1], Position[2] );
 // printf(" orient x:%f y:%f z:%f \n", Orientation[0], Orientation[1], Orientation[2] );

#ifdef NO
 //struct 
 //{
 // long Latitude; 
 // long Longitude; 
 // float Position_X; 
 // float Position_Z; 
 // float Altitude_feet;
 // float Altitude_meters;
 // float Pitch_Deg;
 // float Yaw_Deg;
 // float Roll_Deg;
 //}Aero_POSE;
#endif

#if 0
 if( Aero.AirSpeed_knots < 10 ) Aero.AirSpeed_knots = 10;
 if( Aero.Pitch_deg < 0 ) Aero.Pitch_deg += 360;
 if( Aero.Roll_deg < 0 ) Aero.Roll_deg += 360; 
 if( Aero.Heading_deg < 0 ) Aero.Heading_deg += 360;
 if( Aero.Altitude_AGL_feet < 0 ) Aero.Altitude_AGL_feet = 0;

 if( Aero.AirSpeed_knots > 600 ) Aero.AirSpeed_knots = Aero.AirSpeed_knots_max;
 if( Aero.Pitch_deg > 360 ) Aero.Pitch_deg -= 360;
 if( Aero.Roll_deg > 360 ) Aero.Roll_deg -= 360; 
 if( Aero.Heading_deg > 360 ) Aero.Heading_deg -= 360;
 if( Aero.Altitude_AGL_feet > 90000 ) Aero.Altitude_AGL_feet = 90000;
#endif


 // printf(" Aero.Altitude_meters %f ", Aero.Altitude_meters);
}
