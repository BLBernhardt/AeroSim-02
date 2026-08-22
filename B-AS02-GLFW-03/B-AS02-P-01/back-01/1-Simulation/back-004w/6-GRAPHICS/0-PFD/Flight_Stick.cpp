#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include "../../structures.h"

//#include <Base/ogcGaugeComponent.h>
//extern struct Cntrls_t;
//extern struct _Display  Display;

void  stroke_output(GLfloat, GLfloat, GLfloat, char*,  Cntrls_t *CTRL );

void Flight_Stick( int Pos_X, int Pos_Y, Cntrls_t* CTRL )
{

//char string[40];
//float size = 2.4;

// int window_x;
// int window_y;
int enabled = 0;
  	//  printf("st\n");
  	int pnt_x = 207.0, pnt_y = 207.0;  
  
  
  
	//  float incr = 0.1 * size;
	static float x_pos = 0, y_pos = 0, z_pos = 0;
	static int timer;


 	if( !CTRL->Stick_E )
 	{ 
 		if( CTRL->Stick_R > 0 ) CTRL->Stick_R -= 0.1;
 		if( CTRL->Stick_R < 0 ) CTRL->Stick_R += 0.1;
 		if( CTRL->Stick_P > 0 ) CTRL->Stick_P -= 0.1;
 		if( CTRL->Stick_P < 0 ) CTRL->Stick_P += 0.1; 	
 		if( CTRL->Stick_Y > 0 ) CTRL->Stick_Y -= 0.1;
 		if( CTRL->Stick_Y < 0 ) CTRL->Stick_Y += 0.1;  		
 
 	}
		//printf( ">>>>67 %d \n", CTRL->Stick_Y  );
	x_pos = (float)(+CTRL->Stick_R * 2.0 );
	y_pos = (float)(-CTRL->Stick_P * 2.0 ); 	
	z_pos = (float)(+CTRL->Stick_Y * 2.0); 	

   	if( x_pos >  +100.0 ) x_pos =  +100.0;
  	if( x_pos <  -100.0 ) x_pos =  -100.0;  
  	if( y_pos >  +100.0 ) y_pos =  +100.0; 
  	if( y_pos <  -100.0 ) y_pos =  -100.0; 	
  	if( z_pos >  +100.0 ) z_pos =  +100.0; 
  	if( z_pos <  -100.0 ) z_pos =  -100.0; 	

  	CTRL->R_cmd = +x_pos/100.0;   //   range -1.0 to + 1.0
  	CTRL->P_cmd = -y_pos/100.0;   //   range -1.0 to + 1.0   // Negate to fix window top to bottom corrodinates. 
  	CTRL->Y_cmd = +z_pos/100.0;   //   range -1.0 to + 1.0    	



  	CTRL->Elevator_Cmd  = CTRL->P_cmd;  
  	CTRL->Aileron_Cmd   = CTRL->R_cmd;  
  	CTRL->Rudder_Cmd    = CTRL->Y_cmd;    	
  	
//  	printf(">>>  %d \n", CTRL->MouseZ );
  //	CTRL->Rudder_Cmd +=  

	//printf(" >>>> %f %f %f \n", CTRL->R_cmd, CTRL->P_cmd,   CTRL->Y_cmd );


	timer++;
	if( timer >= 10 )
	{
		timer = 0;
   	//	printf(">>> %.2f %.2f %.2f %.2f %d \n", CTRL->Stick_R, CTRL->Stick_P, CTRL->Stick_R/50.0, CTRL->Stick_P/50.0, CTRL->Stick_Enable );
	}
	
 	// printf(">>>< %f %f \n", x_pos, y_pos );
  
  
	glPushMatrix();
		glTranslatef( Pos_X + pnt_x* 0.5, Pos_Y+pnt_y* 0.5, 0 );    //  location of lower left corner + 1/2 the size to get to center

   		if( enabled )
   		{
   			//x_pos = ( 100.0 * size ) * Pos_X;    // Pos_X -1.0 to +1.0
   			//y_pos = ( 100.0 * size ) * Pos_Y;    // Pos_Y -1.0 to +1.0
   		}
   
  		glColor3f( 0.08, 0.1, 0.2 );            

  		glBegin(GL_POLYGON);         //  Background color
    		glVertex2f( -pnt_x * 0.5 , -pnt_y * 0.5 );
    		glVertex2f( -pnt_x * 0.5 , +pnt_y * 0.5 );
    		glVertex2f( +pnt_x * 0.5 , +pnt_y * 0.5 );
    		glVertex2f( +pnt_x * 0.5,  -pnt_y * 0.5 );
  		glEnd();

  		glColor3f(  0.5, 0.5, 0.5 );

  		glLineWidth(2.0);

  		glBegin(GL_LINE_LOOP);     //  Frame 
    		glVertex2f( -pnt_x * 0.5 , -pnt_y * 0.5 );
    		glVertex2f( -pnt_x * 0.5 , +pnt_y * 0.5 );
    		glVertex2f( +pnt_x * 0.5 , +pnt_y * 0.5 );
    		glVertex2f( +pnt_x * 0.5,  -pnt_y * 0.5 );
  		glEnd();
  		
    	glBegin(GL_LINE_LOOP);      // Center marker
    		glVertex2f( -10, -10 );
    		glVertex2f( -10, +10 );
    		glVertex2f( +10, +10 );
    		glVertex2f( +10, -10 );
  		glEnd();

		// Rudder Center Mark
  		glLineWidth(2.0);
   		glBegin(GL_LINES);
     		glVertex2f(    0,  -pnt_y * 0.5  );
     		glVertex2f(    0,  -pnt_y * 0.5 + 15 );        
   		glEnd();

//printf(" %f %f \n", x_pos, y_pos );

    	glColor3f( 1.0,0,0);
   		glLineWidth(2.0); 		
  		glBegin(GL_LINE_LOOP);      // Cursor 
    		glVertex2f( -4.0 + x_pos,  -4.0 + y_pos );
    		glVertex2f( -4.0 + x_pos,   4.0 + y_pos );
    		glVertex2f(  4.0 + x_pos,   4.0 + y_pos );
    		glVertex2f(  4.0 + x_pos,  -4.0 + y_pos );
  		glEnd();
  		
  		// Rudder deflection marker
   		glBegin(GL_LINES);
     		glVertex2f(    z_pos ,  -pnt_y * 0.5  );
     		glVertex2f(    z_pos ,  -pnt_y * 0.5 + 15 );        
   		glEnd();


glPopMatrix();

}
