#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>

#include "../../structures.h"

//void  stroke_output(GLfloat, GLfloat, GLfloat, char*,  Cntrls_t *Cntrls );

void Flight_Stick( int Pos_X, int Pos_Y, Cntrls_t *Cntrls )
{
	int enabled = 0;
  	int pnt_x = 207.0, pnt_y = 207.0;  
	static float x_pos = 0, y_pos = 0, z_pos = 0;
	static int timer;

    glDisable( GL_CULL_FACE );
//===================================================================================================
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

  		glBegin(GL_LINE_LOOP);     			//  Frame 
    		glVertex2f( -pnt_x * 0.5 , -pnt_y * 0.5 );
    		glVertex2f( -pnt_x * 0.5 , +pnt_y * 0.5 );
    		glVertex2f( +pnt_x * 0.5 , +pnt_y * 0.5 );
    		glVertex2f( +pnt_x * 0.5,  -pnt_y * 0.5 );
  		glEnd();
  		
    	glBegin(GL_LINE_LOOP);      		// Center marker
    		glVertex2f( -10, -10 );
    		glVertex2f( -10, +10 );
    		glVertex2f( +10, +10 );
    		glVertex2f( +10, -10 );
  		glEnd();

  		glLineWidth(2.0);					// Rudder Center Mark
   		glBegin(GL_LINES);
     		glVertex2f(    0,  -pnt_y * 0.5  );
     		glVertex2f(    0,  -pnt_y * 0.5 + 15 );        
   		glEnd();

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
