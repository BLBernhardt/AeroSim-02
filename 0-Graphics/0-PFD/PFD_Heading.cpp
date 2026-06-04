
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <math.h>


extern void stroke_output(GLfloat, GLfloat, GLfloat, const char* );

void PFD_Heading( int Pos_X, int Pos_Y, float Heading )
{
 char string[20];
 int i;
 int ix, iy, isize;

 float theta, st, ct;
 float start, x, y, end;


	ix = 0;
 	iy = 0;
	isize = 450;



 	if( Heading <   0 ) Heading = Heading + 360;
 	if( Heading <   0 ) Heading = Heading + 360;
 	if( Heading > 360 ) Heading = Heading - 360;
 	if( Heading > 360 ) Heading = Heading - 360;

 	glPushMatrix(); // 1

 		glTranslatef( Pos_X, Pos_Y, 0 );

		start = M_PI * ( 125.0f ) / 180.0f;
		end   = M_PI * ( 235.0f ) / 180.0f;
 		glColor3ub( 21,21,46); // Color gray-blue			back ground 
 		glBegin(GL_POLYGON);
 			for (theta = start; theta <= end; theta += M_PI / 30 )
 		{
 			x = -sin(theta) * isize;
 			y = (-isize * sin(start)) -cos(theta) * isize; 
 			glVertex2f( x, y );
 		}
 		glEnd();

 		// glRotatef( IndicatorAngle, 0.0, 0.0, 1.0 );
 
 		glColor3f(0.8f, 0.8f, 0.8f);

 		isize = 900;
 		glPushMatrix(); //2
 		glTranslatef( 0, -isize/ 2.0 * sin(start), 0 );

 		int hdg = 30;
 		//	glPushMatrix(); //3
 				if( Heading < 0 ) Heading = Heading + 360;
 				glRotatef( Heading, 0.0, 0.0, 1.0 );

 				for (i = 360; i > 0; i -= 30)
 				{
 					sprintf( string, "%i", i );
 					if( i >= 100 ) stroke_output( ix + isize * -0.036f, iy + isize * 0.428f, .22, string );
 					else if( i >= 10 ) stroke_output( ix + isize * -0.021f, iy + isize * 0.428f, .22, string );
 					else stroke_output( ix + isize * -0.010f, iy + isize * 0.428f, .22, string );
 					glRotatef( 30, 0.0, 0.0, 1.0 );
 				}
 						
 	 			iy = 0;
 				glLineWidth(2.0);
 				glBegin(GL_LINES);					
  					for (i = 0; i <= 360; i += 10)
 					{
 						theta = i * M_PI/ 180;
	 					st = sin(theta);
	 					ct = cos(theta);

	 					glVertex2f( ix + st * 0.47f * isize , iy + ct * 0.47f * isize );
	 					glVertex2f( ix + st * 0.50f * isize , iy + ct * 0.5f * isize );
 					}						
 				glEnd();	

		 		glLineWidth(1.0);
 				glBegin(GL_LINES);
 					for (i = 5; i <= 360; i += 10)
 					{
 						theta = i * M_PI/ 180;
		 				st = sin(theta);
		 				ct = cos(theta);

		 				glVertex2f( ix + st * 0.48f * isize, iy + ct * 0.48f * isize );
			 			glVertex2f( ix + st * 0.5f * isize, iy + ct * 0.5f * isize );
 					}
 				glEnd();
	
	 			glLineWidth(3.0);
	 			glBegin(GL_LINES);
	 				for (i = 0; i <= 360; i += 30)
	 				{
	 					theta = i * M_PI/ 180;
		 				st = sin(theta);
		 				ct = cos(theta);

		 				glVertex2f( ix + st * 0.46f * isize, iy + ct * 0.46f * isize );
		 				glVertex2f( ix + st * 0.5f * isize, iy + ct * 0.5f * isize );
	 				}
	 			glEnd();

 			glPopMatrix(); //2

 			glColor3f(0.0f, 0.0f, 0.0f);
 			glBegin(GL_POLYGON);
 				glVertex2f( -700, -200 );
 				glVertex2f( -700, -100 );
 				glVertex2f( 700, -100 );
 				glVertex2f( 700, -200 );
 			glEnd();


 		glTranslatef( 0, 110, 0 );

 		float size = 4.0;
 		glColor3f( 1, 1, 1 );
 		glLineWidth(2.0);
 		glBegin(GL_LINE_LOOP);
 			glVertex2f( size* -7.0, size* 0.0 );
 			glVertex2f( size* 7.0, size* 0.0 );
 			glVertex2f( size* 0.0, size* -4.5 );
 		glEnd();

 		glTranslatef( 0, 30, 0 );

 		glTranslatef( 0, -230, 0 );
 		glColor3f( 0.0, 0.0, 0.0 ); // Draw black background
 		glBegin(GL_POLYGON);
 			glVertex2f( size* -11.0, size* 0.0 );
 			glVertex2f( size* -11.0, size* 16.0 );
 			glVertex2f( size* 11.0, size* 16.0 );
 			glVertex2f( size* 11.0, size* 0.0 );
 		glEnd();



 		glColor3f( 0.0, 0.6, 0.0 );
 		glBegin(GL_LINE_LOOP);
 			glVertex2f( size* -11.0, size* 0.0 );
 			glVertex2f( size* -11.0, size* 16.0 );
 			glVertex2f( size* 11.0, size* 16.0 );
 			glVertex2f( size* 11.0, size* 0.0 );
 		glEnd();

	
 		glColor3f( 1.0, 1.0, 1.0 );
 		sprintf( string, "%i", (int)Heading );
 
 		ix = -53;
 		iy = -10;

 		float fontHeight = .3;
 		hdg = (int) Heading; 

 		// 100's
 		sprintf( string, "%i", hdg/100);
 		stroke_output( ix + 14, iy + 27, fontHeight, string );
 		hdg = hdg-100*(int)(hdg/100);
 
 		// 10's
 		sprintf( string, "%i", hdg/10);
 		stroke_output( ix + 42, iy + 27, fontHeight, string );
 		hdg = hdg-10*(int)(hdg/10);

	 	// 1's
 		sprintf( string, "%i", hdg);
 		stroke_output( ix + 70, iy + 27, fontHeight, string );


	glPopMatrix(); //1

	
}













//endfile 







