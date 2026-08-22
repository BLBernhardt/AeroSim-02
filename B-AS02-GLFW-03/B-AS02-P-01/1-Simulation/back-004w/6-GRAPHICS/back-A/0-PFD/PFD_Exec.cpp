#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include "../../structures.h"


void stroke_output(GLfloat, GLfloat, GLfloat, const char* );

//extern struct Display_t DSPLg;
//extern struct Aerodyn_t Aero_D;
//extern struct _Cntrls Cntrls;

void PFD_Exec( int, int );
void PFD_Attitude( int Pos_X, int Pos_Y, Aerodyn_t* AERO_D );
void PFD_SpeedTicker( int, int, int, int, int, int, float );
void PFD_AltTicker( int, int, int, int, int, int, float );
void PFD_SpeedTape( int, int, float );
void PFD_AltTape( int, int, float );
void PFD_RateOfClimb( int, int, float );
//void PFD_Bezzel( int, int );
void PFD_Heading( int, int, float );
void Flight_Stick( int Pos_X, int Pos_Y, Cntrls_t *Cntrls );
void PFD_Display( Aerodyn_t* AERO_D );



void PFD_Exec( int offset_X , int offset_Y, Cntrls_t *Cntrls, Aerodyn_t *AERO_D, Display_t* DSPL )
{
 int DG_X, DG_Y;
 int Pos_X, Pos_Y;
 //float scale;
 //int offset_X, offset_Y;
 
 //printf("24 %d %d \n", offset_X, offset_Y );
 DG_X = DSPL->Grid_X;
 DG_Y = DSPL->Grid_Y;
 
 		int X = 100;
		glPushMatrix();
			glTranslatef( (float)offset_X, 150, 0 );
			
			
		
	
 			Pos_X = 68 - X; Pos_Y = 0 ;
 			
 			Pos_X = 275 - X; Pos_Y = 0 ; // -56
			PFD_AltTicker( Pos_X, Pos_Y, DG_X, DG_Y, offset_X, offset_Y, AERO_D->Altitude_AGL_feet ); 			
 #if 0				
 	 		AERO_D->Roll_d = 30.0;
			PFD_Attitude( Pos_X, Pos_Y, AERO_D );

			Pos_X = 320 - X; Pos_Y = 0 ; // -353
			PFD_AltTape( Pos_X, Pos_Y, AERO_D->Altitude_AGL_feet ); 
			
			Pos_X = 275 - X; Pos_Y = 0 ; // -56
			PFD_AltTicker( Pos_X, Pos_Y, DG_X, DG_Y, offset_X, offset_Y, AERO_D->Altitude_AGL_feet ); 
			
			Pos_X = 520 - X; Pos_Y = 0 ;
			PFD_RateOfClimb( Pos_X, Pos_Y, AERO_D->RateOfClimb_ft_sec ); 			
			
			Pos_X = -390 - X; Pos_Y = -350 ;
			PFD_SpeedTape( Pos_X, Pos_Y, AERO_D->AirSpeed_knots ); 
			
			Pos_X = -390 - X; Pos_Y = -48 ;
			PFD_SpeedTicker( Pos_X, Pos_Y, DG_X, DG_Y, offset_X, offset_Y, AERO_D->AirSpeed_knots );

			Pos_X = 0 - X; Pos_Y = -500 ;
			PFD_Heading( Pos_X, Pos_Y, AERO_D->Yaw_d );
			
			Pos_X = 400 - X; Pos_Y = -600 ;
			Flight_Stick( Pos_X, Pos_Y, Cntrls );

			//PFD_Bezzel( Pos_X, Pos_Y );
#endif
			#if 0
 		glColor3f( 0.2, 0.2, 0.2 );
 		glLineWidth(1.0);

 		glBegin(GL_LINE_LOOP);
 		glVertex2f( -500, -500);
 		glVertex2f( -500, 500);
 		glVertex2f( 500, 500);
 		glVertex2f( 500, -500);
 		glEnd();
#endif
 glPopMatrix();


 	
}
