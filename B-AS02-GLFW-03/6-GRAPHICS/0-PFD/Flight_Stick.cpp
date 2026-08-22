#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>

#include "../../structures.h"
#include "../../IPC_CONFIG.h"

void  stroke_output(GLfloat, GLfloat, GLfloat, char*,  Cntrls_t *CTRL );

void Flight_Stick( int Pos_X, int Pos_Y, Cntrls_t *CTRL )
{
//	int enabled = 0;
  	//  printf("st\n");
  	int pnt_x = 207.0, pnt_y = 207.0;  
	static float x_pos = 0, y_pos = 0, z_pos = 0;

  		// printf(">>>> %d \n",  CTRL->Stick_E );

 		if( !CTRL->Stick_E )
 		{ 
 			if( CTRL->Stick_R > 0 ) CTRL->Stick_R -= 0.1;
 			if( CTRL->Stick_R < 0 ) CTRL->Stick_R += 0.1;
 			if( CTRL->Stick_P > 0 ) CTRL->Stick_P -= 0.1;
 			if( CTRL->Stick_P < 0 ) CTRL->Stick_P += 0.1; 	
 			if( CTRL->Stick_Y > 0 ) CTRL->Stick_Y -= 0.1;
 			if( CTRL->Stick_Y < 0 ) CTRL->Stick_Y += 0.1;  	
 			if( CTRL->MouseZ  > 0 ) CTRL->MouseZ  -= 1.0;
 			if( CTRL->MouseZ  < 0 ) CTRL->MouseZ  += 1.0;	
 		}
 		
		x_pos = (float)(+CTRL->Stick_R * 2.0 );
		y_pos = (float)(-CTRL->Stick_P * 2.0 ); 	
		z_pos = (float)(+CTRL->MouseZ  * 2.0 );	

   		if( x_pos >  +100.0 ) x_pos =  +100.0;
  		if( x_pos <  -100.0 ) x_pos =  -100.0;  
  		if( y_pos >  +100.0 ) y_pos =  +100.0; 
  		if( y_pos <  -100.0 ) y_pos =  -100.0; 	
  		if( z_pos >  +100.0 ) z_pos =  +100.0; 
  		if( z_pos <  -100.0 ) z_pos =  -100.0; 	

  		CTRL->Elv_Cmd_u  = +y_pos/100.0;   //   range -1.0 to + 1.0     // Negate to fix window top to bottom corrodinates. 
  		CTRL->Ail_Cmd_u  = +x_pos/100.0;   //   range -1.0 to + 1.0   
		CTRL->Rud_Cmd_u  = +z_pos/100.0;   //   range -1.0 to + 1.0 
		 
		// printf(" %f %f %f \n",  CTRL->Elv_Cmd_u, CTRL->Ail_Cmd_u, CTRL->Rud_Cmd_u ); 
		// printf(" %f %d CTRL->MouseZ  \n", CTRL->Rud_Cmd_u, CTRL->MouseZ   );

		glPushMatrix();
			glTranslatef( Pos_X + pnt_x* 0.5, Pos_Y+pnt_y* 0.5, 0 );	//  location of lower left corner + 1/2 the size to get to center
  
  			glColor3f( 0.08, 0.1, 0.2 );			

  			glBegin(GL_POLYGON);		 //  Background color
				glVertex2f( -pnt_x * 0.5 , -pnt_y * 0.5 );
				glVertex2f( -pnt_x * 0.5 , +pnt_y * 0.5 );
				glVertex2f( +pnt_x * 0.5 , +pnt_y * 0.5 );
				glVertex2f( +pnt_x * 0.5,  -pnt_y * 0.5 );
  			glEnd();

  			glColor3f(  0.5, 0.5, 0.5 );

  			glLineWidth(2.0);

  			glBegin(GL_LINE_LOOP);	 //  Frame 
				glVertex2f( -pnt_x * 0.5 , -pnt_y * 0.5 );
				glVertex2f( -pnt_x * 0.5 , +pnt_y * 0.5 );
				glVertex2f( +pnt_x * 0.5 , +pnt_y * 0.5 );
				glVertex2f( +pnt_x * 0.5,  -pnt_y * 0.5 );
  			glEnd();
  		
			glBegin(GL_LINE_LOOP);	  // Center marker
				glVertex2f( -10, -10 );
				glVertex2f( -10, +10 );
				glVertex2f( +10, +10 );
				glVertex2f( +10, -10 );
  			glEnd();

			//printf(" %f %f \n", x_pos, y_pos );
			glColor3f( 1.0,0,0);
   			glLineWidth(2.0); 		
  			glBegin(GL_LINE_LOOP);	  // Cursor 
				glVertex2f( -4.0 + x_pos,  -4.0 + y_pos );
				glVertex2f( -4.0 + x_pos,   4.0 + y_pos );
				glVertex2f(  4.0 + x_pos,   4.0 + y_pos );
				glVertex2f(  4.0 + x_pos,  -4.0 + y_pos );
  			glEnd();

			// Rudder Center Mark
  			glLineWidth(2.0);
   			glBegin(GL_LINES);
	 			glVertex2f(	0,  -pnt_y * 0.5  );
	 			glVertex2f(	0,  -pnt_y * 0.5 + 15 );		
   			glEnd();

  			// Rudder deflection marker
   			glBegin(GL_LINES);
		 		glVertex2f(	z_pos ,  -pnt_y * 0.5  );
		 		glVertex2f(	z_pos ,  -pnt_y * 0.5 + 15 );		
   			glEnd();
		glPopMatrix();

}


//========================================================================================
// eof



