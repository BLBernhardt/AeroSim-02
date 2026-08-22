#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include "../../structures.h"

//#include <Base/ogcGaugeComponent.h>
//extern struct Cntrls_t;
//extern struct _Display  Display;

void  stroke_output(GLfloat, GLfloat, GLfloat, char*,  Cntrls_t *Cntrls );

void Flight_Stick( int Pos_X, int Pos_Y, Cntrls_t *Cntrls )
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


 	if( !Cntrls->Stick_Enable )
 	{ 
 		if( Cntrls->Stick_X > 0 ) Cntrls->Stick_X -= 0.1;
 		if( Cntrls->Stick_X < 0 ) Cntrls->Stick_X += 0.1;
 		if( Cntrls->Stick_Y > 0 ) Cntrls->Stick_Y -= 0.1;
 		if( Cntrls->Stick_Y < 0 ) Cntrls->Stick_Y += 0.1; 	
 		if( Cntrls->Stick_Z > 0 ) Cntrls->Stick_Z -= 0.1;
 		if( Cntrls->Stick_Z < 0 ) Cntrls->Stick_Z += 0.1;  		
 	}

	x_pos = (float)(+Cntrls->Stick_X );
	y_pos = (float)(-Cntrls->Stick_Y ); 	
	z_pos = (float)(+Cntrls->Stick_Z * 2.0); 	

   	if( x_pos >  +100.0 ) x_pos =  +100.0;
  	if( x_pos <  -100.0 ) x_pos =  -100.0;  
  	if( y_pos >  +100.0 ) y_pos =  +100.0; 
  	if( y_pos <  -100.0 ) y_pos =  -100.0; 	
  	if( z_pos >  +100.0 ) z_pos =  +100.0; 
  	if( z_pos <  -100.0 ) z_pos =  -100.0; 	

  	Cntrls->x_cmd = +x_pos/100.0;   //   range -1.0 to + 1.0
  	Cntrls->y_cmd = -y_pos/100.0;   //   range -1.0 to + 1.0   // Negate to fix window top to bottom corrodinates. 
  	Cntrls->z_cmd = +z_pos/100.0;   //   range -1.0 to + 1.0    	



  	Cntrls->Elevator_Cmd  = Cntrls->y_cmd;  
  	Cntrls->Aileron_Cmd   = Cntrls->x_cmd;  
  	Cntrls->Rudder_Cmd    = Cntrls->z_cmd;    	
  	
//  	printf(">>>  %d \n", Cntrls->MouseZ );
  //	Cntrls->Rudder_Cmd +=  

	//printf(" >>>> %f %f %f \n", Cntrls->x_cmd, Cntrls->y_cmd,   Cntrls->z_cmd );


	timer++;
	if( timer >= 10 )
	{
		timer = 0;
   	//	printf(">>> %.2f %.2f %.2f %.2f %d \n", Cntrls->Stick_X, Cntrls->Stick_Y, Cntrls->Stick_X/50.0, Cntrls->Stick_Y/50.0, Cntrls->Stick_Enable );
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
