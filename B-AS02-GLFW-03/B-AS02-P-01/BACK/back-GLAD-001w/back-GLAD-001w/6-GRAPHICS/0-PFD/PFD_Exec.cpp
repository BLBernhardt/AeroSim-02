#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include "../../structures.h"


void  stroke_output(GLfloat, GLfloat, GLfloat, const char* );

extern struct Display_t  DSPLg;
extern struct Aerodyn_t  Aero_D;
//extern struct _Cntrls   Cntrls;

void PFD_Exec( int, int );
void PFD_Attitude(     int, int, float, float        );
void PFD_SpeedTicker(  int, int, int,   int,  int, int,  float );
void PFD_AltTicker(    int, int, int,   int, int, int,   float );
void PFD_SpeedTape(    int, int, float               );
void PFD_AltTape(      int, int, float               );
void PFD_RateOfClimb(      int, int, float               );
//void PFD_Bezzel(       int, int                      );
void PFD_Heading(      int, int, float               );
void Flight_Stick( int Pos_X, int Pos_Y, Cntrls_t *Cntrls  );


void PFD_Exec( int offset_X , int offset_Y, Cntrls_t *Cntrls, Aerodyn_t *Aero_D )
{
 int  DG_X, DG_Y;
 int Pos_X, Pos_Y;
 //float scale;
 //int offset_X, offset_Y;
  
 //printf("24 %d %d \n", offset_X, offset_Y );
        DG_X = DSPLg.Grid_X;
        DG_Y = DSPLg.Grid_Y;
  
  		int X = 150;
		glPushMatrix();
			glTranslatef( (float)offset_X, 150, 0 );
		
        	Pos_X = 0 - X;  Pos_Y = 0 ;
			PFD_Attitude( Pos_X, Pos_Y, Aero_D->Pitch_d, Aero_D->Roll_d );

			Pos_X = 320 - X;  Pos_Y = 0 ; // -353
			PFD_AltTape(  Pos_X, Pos_Y,    Aero_D->Altitude_AGL_feet  );  
			
			
			Pos_X = 520 - X;  Pos_Y = 0 ;
			PFD_RateOfClimb(  Pos_X, Pos_Y,   Aero_D->RateOfClimb_ft_sec  );  			


			Pos_X = 275 - X;   Pos_Y = 0 ;   // -56
			PFD_AltTicker(   Pos_X, Pos_Y, DG_X, DG_Y, offset_X, offset_Y, Aero_D->Altitude_AGL_feet  );  
			
			Pos_X = -390 - X;  Pos_Y = -350 ;
			PFD_SpeedTape(  Pos_X, Pos_Y,  Aero_D->AirSpeed_knots ); 
			
			Pos_X = -390 - X;  Pos_Y = -48 ;
			PFD_SpeedTicker(  Pos_X, Pos_Y, DG_X, DG_Y, offset_X, offset_Y,  Aero_D->AirSpeed_knots );

			Pos_X = 0 - X;  Pos_Y = -520 ;
			PFD_Heading( Pos_X, Pos_Y, Aero_D->Yaw_d  );
			
			Pos_X = 340 - X;  Pos_Y = -580 ;
			Flight_Stick( Pos_X, Pos_Y, Cntrls );

			//PFD_Bezzel( Pos_X, Pos_Y );
			
       		glColor3f( 0.2, 0.2, 0.2 );
      		glLineWidth(1.0);

       		glBegin(GL_LINE_LOOP);
         		glVertex2f( -500, -500);
         		glVertex2f( -500,  500);
         		glVertex2f(  500,  500);
         		glVertex2f(  500, -500);
       		glEnd();

      glPopMatrix();


  	
}
