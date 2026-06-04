
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>

void stroke_output(GLfloat, GLfloat, GLfloat, const char* );

void PFD_RateOfClimb( int Pos_X, int Pos_Y, float ROC )
{

char string[50];
float size = 5.0;

//int m_PhysicalSize_x = size* 34; 
int height 	= size * 100;
int width 	= size * 20;


//float tickLocation = 0; 
//float tickSpacing = size* 11.3;
//float tickWidth = size* 3.7;
float fontHeight = size* 5.0;

glPushMatrix();
 glTranslatef( Pos_X, Pos_Y - height/2.0, 0 );

 // Draw the background rectangle

 glColor3ub(51,51,76); // Color gray-blue

 glBegin(GL_POLYGON);
 	glVertex2f( 0.0			, 0 				);
 	glVertex2f( 0.0			, height 		);
 	glVertex2f( width * 0.3	, height 		);
 	glVertex2f( width			, height * 0.75 	);
 	glVertex2f( width			, height * 0.25 	); 
 	glVertex2f( width * 0.3	, 0 				); 
 glEnd();
 
 glColor3f( 1, 1, 1 ); // Color gray-blue
#if 1
	float roc = 2.0 * 10.0, y1, y2;
	glBegin( GL_LINES );
		glVertex2f(  30.0,  height * ( 0.95  ));
		glVertex2f(  40.0,  height * ( 0.95  ));		
		
		glVertex2f(  30.0,  height * ( 0.92  ));
		glVertex2f(  40.0,  height * ( 0.92  ));	

		glVertex2f(  30.0,  height * ( 0.88  ));
		glVertex2f(  40.0,  height * ( 0.88  ));	
		
		glVertex2f(  30.0,  height * ( 0.81  ));
		glVertex2f(  40.0,  height * ( 0.81  ));
		
		glVertex2f(  30.0,  height * ( 0.73  ));
		glVertex2f(  40.0,  height * ( 0.73  ));		
		
		glVertex2f(  30.0,  height * ( 0.62  ));
		glVertex2f(  40.0,  height * ( 0.62  ));
		
		
		glVertex2f(  30.0,  height * ( 0.50  ));
		glVertex2f(  40.0,  height * ( 0.50  ));
		
		
		glVertex2f(  30.0,  height * ( 0.38  ));
		glVertex2f(  40.0,  height * ( 0.38  ));
		
		glVertex2f(  30.0,  height * ( 0.27  ));
		glVertex2f(  40.0,  height * ( 0.27  ));		
		
		glVertex2f(  30.0,  height * ( 0.19  ));
		glVertex2f(  40.0,  height * ( 0.19  ));
		
		glVertex2f(  30.0,  height * ( 0.12  ));
		glVertex2f(  40.0,  height * ( 0.12  ));		
			
		glVertex2f(  30.0,  height * ( 0.08  ));
		glVertex2f(  40.0,  height * ( 0.08  ));
		
		glVertex2f(  30.0,  height * ( 0.05  ));
		glVertex2f(  40.0,  height * ( 0.05  ));
		
	glEnd();

#endif	
 	fontHeight = .2;
	glLineWidth(2.0);
	sprintf( string, "%i", 6 );
 	stroke_output( 5, height * (0.98 - 0.04) , fontHeight, string );
 	sprintf( string, "%i", 2 );
 	stroke_output( 5, height * (0.90 - 0.04) , fontHeight, string );
 	sprintf( string, "%i", 1 );
 	stroke_output( 5, height * (0.75 - 0.04) , fontHeight, string );
 	
 	sprintf( string, "%i", 0 );
 	stroke_output( 5, height * (0.50 - 0.02) , fontHeight, string );
 	
 	sprintf( string, "%i", 1 );
 	stroke_output( 5, height * (0.25      ) , fontHeight, string );
 	sprintf( string, "%i", 2 );
 	stroke_output( 5, height * (0.10      ) , fontHeight, string );
 	sprintf( string, "%i", 6 );
 	stroke_output( 5, height * (0.02      ) , fontHeight, string );

	roc = ROC/1000;	
	y1 = 230.0 * ( ( -0.000167*roc*roc*roc) + ( -0.0595 * roc * roc) + ( 0.5297 * roc ));
	y2 = y1 * 0.6;
	glLineWidth(4.0);
	glBegin( GL_LINES );
		glVertex3f(  45.0, y1 + height/2.0, 0 );
		glVertex3f( 100.0, y2 + height/2.0, 0 );		
	glEnd();




 glPopMatrix();
}

