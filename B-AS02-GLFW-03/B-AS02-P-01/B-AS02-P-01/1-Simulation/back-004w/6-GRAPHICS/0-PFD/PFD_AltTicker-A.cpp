
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include "../../structures.h"

extern struct _Display Display;


void  stroke_output(GLfloat x, GLfloat y, GLfloat size, const char *text );



void PFD_AltTicker( int Pos_X, int Pos_Y, int Disp_Area_X, int Disp_Area_Y, int offset_X, int offset_Y, float ALT )
{

char string[40];


int height = 45;


int alt;
//int altitude;
int digit;
int window_x, window_y;
int alt2;

float size = 5.0;
float i;
float vertOffset;
float tickLocation = 0; 
float tickSpacing = size* 3.5;
//float tickWidth = size* 3.7;
float fontHeight = .30;

	
 	glPushMatrix();
 		glTranslatef( Pos_X, Pos_Y - height, 0 ); 

 		// The speed tape doesn't display speeds < 30 or > 90,000
 		if( ALT < 30.0 ) ALT = 30.0 ;
 		if( ALT > 90000.0 ) ALT = 90000.0 ; 

 		alt = (int)ALT;
 		
 		#if 1
 		glColor3ub( 0, 0, 1 ); // Draw black background
 		glBegin(GL_POLYGON);
 		glVertex2f( size* 30.0, size* 18.0 );
 		glVertex2f( size* 32.0, size* 21.0 );
 		glVertex2f( size* 45.0, size* 21.0 );
 		glVertex2f( size* 47.0, size* 18.0 );
 		glVertex2f( size* 47.0, size* 0.0 );
 		glVertex2f( size* 45.0, size* -3.0 );
 		glVertex2f( size* 32.0, size* -3.0 );
 		glVertex2f( size* 30.0, size* 0.0 );
 		glEnd();
 		glColor3f( 0, 0, 0 );
 		glBegin(GL_POLYGON);
 		glVertex2f( size* 15.5, size* 0.75 );
 		glVertex2f( size* 15.5, size* 17.0 );
 		glVertex2f( size* 30.0, size* 17.0 );
 		glVertex2f( size* 30.0, size* 0.75 );
 		glEnd();

 		glBegin(GL_TRIANGLES);
 		glVertex2f( size* 11.0, size* 9.0 );
 		glVertex2f( size* 15.5, size* 14.0 );
 		glVertex2f( size* 15.5, size* 4.0 );
 		glEnd();
	   #endif


 		// 10,000's
 		fontHeight = 0.30;
 		glColor3f( 1.0, 1.0, 1.0 );
 		glLineWidth( 2.0 ); 		
 		if(alt >= 10000)
 		{
 			sprintf( string, "%i", (int)(alt/10000) );
 			stroke_output( 82, 30, fontHeight, string );
 		}
 		else // draw a green square
 		{
 		glColor3f( 0.0, 0.6, 0 );
 		glBegin(GL_POLYGON);
 			glVertex2f( size* 17.0, size* 5.0 );
 			glVertex2f( size* 17.0, size* 13.0 );
 			glVertex2f( size* 21.0, size* 13.0 );
 			glVertex2f( size* 21.0, size* 5.0 );
 		glEnd();
 		}

 		glColor3f( 1, 1, 1 );

 		// 1,000's
 		alt = alt - 10000*(int)(alt/10000);
 		digit = (int)alt/1000;
 		sprintf( string, "%d", digit );
 		stroke_output( 110, 30, fontHeight, string );

 		// 100's
 		alt = alt-1000*(int)(alt/1000);
 		digit = (int)alt/100;
 		sprintf( string, "%i", digit );
 		stroke_output( 137, 30, fontHeight, string );

 		// 10's Scrolling
 		alt = (( (int)ALT/20 )*20 - ( (int)ALT/100*100 )) / 10 ;
 		alt = alt - 2;

 		vertOffset = (ALT - (float)((int)(ALT/20)*20))/20.0;

 		
 	//	glfwGetWindowSize( window, &width, &height );
 		
 		
 		window_x = glutGet(GLUT_WINDOW_WIDTH );
 		window_y = glutGet(GLUT_WINDOW_HEIGHT );

 		if( window_x >= window_y ) 
 		{																			// Implemeted better glscissor coordinates for x axis only, need simular for Y 
 			glScissor( 	(float)window_x/2.0 + (float)window_y * (Pos_X + offset_X + 160)/1100, // winddow size see glutGet(GLUT_WINDOW_WIDTH ) glutGet(GLUT_WINDOW_HEIGHT )
 						(float)window_y * 0.58, 									// 1000 is the Display.Grid_X, Display.Grid_Y size controlled by the minor WINDOW axis size. 
	 					(float)window_y * 0.07,
		 				(float)window_y * 0.11 );
 		}
 		else
 		{
 			glScissor( 	(float)window_x * ( -500 - Pos_X + offset_X - 10 )/1100 , // +60 above and -10 because the scissor is outside the 1000 x 1000 draw areea a bit, need to fix this by getting size variable implemented fully
 	 					(float)window_y/2.0 + (float)window_x * 0.033, 
	 	 				(float)window_x * 0.07,
			 			(float)window_x * 0.12 );
 		}

	glEnable( GL_SCISSOR_TEST );	
	
	
#if 0// use for testing 
 		glColor3ub( 100, 0, 0 ); // Draw black background
 		// glRectd(0.0,0.0,18.0,18.0); // Rectangular part
 		glBegin(GL_POLYGON);
 		glVertex2f( -1000, -1000 );
 		glVertex2f( -1000, 1000 );
 		glVertex2f( 1000, 1000 );
 		glVertex2f( 1000, -1000 );
 	glEnd();
#endif 		

 		glColor3f( 1.0, 1.0, 1.0 );

 		for ( i = 0; i <= (float)height + 100; i += (2 * tickSpacing) )
 		{
 			tickLocation = 1.3 * (i - ( 2.0 * vertOffset * tickSpacing ));

 			alt2 = alt;
 			if( alt2 >= 10 ) alt2 = alt - 10;
 			if( ALT >= 50 && alt2 == -2 ) alt2 = 8;

 			sprintf( string, "%d", alt2 );
 			if( ALT < 50 && alt2 == -2 ) sprintf( string, "- " );

 			fontHeight = 0.31;
 			stroke_output( 165, -16 + tickLocation, fontHeight, string );
 			stroke_output( 193, -16 + tickLocation, fontHeight, "0" ); 

 			alt = alt + 2;
 		}

#if 0
 glColor3f( 0, 1, 0 ); 
 glBegin(GL_POLYGON);
 glVertex2f( -700, -700 );
 glVertex2f( -700, 700 );
 glVertex2f( 700, 700 );
 glVertex2f( 700, -700 );

 glEnd();
#endif

 		glDisable( GL_SCISSOR_TEST );
 
 		glPopMatrix();
 	
 	
	//  White Boarder 
 		glPushMatrix();
 			glTranslatef( Pos_X, Pos_Y - height, 0 ); 
 			glColor3f( 1.0, 1.0, 1.0 );
 			glLineWidth(2.0);
 				glBegin(GL_LINE_LOOP);
 	 			glVertex2f( size* 11.0, size* 9.0 );
 	 			glVertex2f( size* 15.5, size* 14.0 );
 	 			glVertex2f( size* 15.5, size* 17.0 );
 	 			glVertex2f( size* 30.0, size* 17.0 );
 	 			glVertex2f( size* 32.0, size* 21.0 );
 	 			glVertex2f( size* 45.0, size* 21.0 );
 	 			glVertex2f( size* 47.0, size* 19.0 );
 	 			glVertex2f( size* 47.0, size* -1.0 );
 	 			glVertex2f( size* 45.0, size* -3.0 );
 	 			glVertex2f( size* 32.0, size* -3.0 );
 	 			glVertex2f( size* 30.0, size* 1.0 );
 	 			glVertex2f( size* 15.5, size* 1.0 );
 	 			glVertex2f( size* 15.5, size* 4.0 );
 				glEnd();
 		glPopMatrix();

}
