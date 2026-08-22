#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include "../../structures.h"

//#include <Base/ogcGaugeComponent.h>


void  stroke_output(GLfloat x, GLfloat y, GLfloat size, const char *text );


extern struct _Display Display;

#if 0
void stroke_output(GLfloat x, GLfloat y, GLfloat size, char *text )
{
 char *p;

 glPushMatrix();
 glTranslatef(x, y, 0);
 glScalef( size, size, size );
 for (p = text; *p; p++)
 glutStrokeCharacter(GLUT_STROKE_ROMAN, *p);
 glPopMatrix();
}
#endif


void PFD_SpeedTicker( int Pos_X, int Pos_Y, float IAS )
{
 char string[40];
 float size = 5.0;

 int window_x;
 int window_y;

	glPushMatrix();
		glTranslatef( Pos_X, Pos_Y, 0 );

 		glColor3ub( 1, 1, 0 ); // Draw black background
 		glBegin(GL_POLYGON);
 			glVertex2f( size* 0.0, size* 0.0 );
 			glVertex2f( size* 0.0, size* 18.0 );
 			glVertex2f( size* 22.0, size* 18.0 );
 			glVertex2f( size* 22.0, size* 0.0 );
 		glEnd();

		#if 0
 		// Triangular part
 		glColor3ub( 1, 0, 0 ); 
 			glBegin(GL_TRIANGLES);
 			glVertex2f( size* 22.0, size* 11.0);
 			glVertex2f( size* 25.0, size* 9.0);
 			glVertex2f( size* 22.0, size* 7.0);
 		glEnd();
		#endif
 	
 		int spd = 0;
 		spd = (int) IAS; 

 		// printf(" %f ", IAS );
 		if( IAS < 0 )
 		{
 			IAS = 0.0;
 			spd = 0;
 		}

 		// double texty = m_PhysicalSize.y / 2 - fontHeight / 2;
 		// m_pFontManager->SetSize(m_Font, 6.0, fontHeight);
 		// Draw text in white

 		glColor3f( 1.0, 1.0, 1.0 );
 		float fontHeight = .3 ;
 		glLineWidth(2.0);

 		// 100's
 		sprintf( string, "%i", spd/100);
 		stroke_output( 9, 27, fontHeight, string );
 		spd = spd-100*(int)(spd/100);

 		// 10's
 		sprintf( string, "%i", spd/10);
 		stroke_output( 37, 27, fontHeight, string );
 		spd = spd-10*(int)(spd/10);

 		// 1's
 		// sprintf( string, "%i", spd/1 );
 		// stroke_output( 37+28, 27, fontHeight, string );
 		// spd = spd-1*(int)(spd/1);

 		float decimal = 0;
 		decimal = -1.1 * ( IAS - (int) IAS );

 	glPopMatrix();

 	glPushMatrix();
 		glTranslatef( Pos_X, Pos_Y, 0 );


 	 	window_x = glutGet(GLUT_WINDOW_WIDTH );
 		window_y = glutGet(GLUT_WINDOW_HEIGHT );

 		if( window_x >= window_y ) 
 		{																			// Implemeted better glscissor coordinates for x axis only, need simular for Y 
 			glScissor( 	(float)window_x/2.0 + (float)window_y * (Pos_X + 60)/1100, // winddow size see glutGet(GLUT_WINDOW_WIDTH ) glutGet(GLUT_WINDOW_HEIGHT )
 						(float)window_y * 0.59, 									// 1000 is the Display.Grid_X, Display.Grid_Y size controlled by the minor WINDOW axis size. 
	 					(float)window_y * 0.04,
		 				(float)window_y * 0.085);
 		}
 		else
 		{
 			glScissor( 	(float)window_x * ( -500 - Pos_X - 10 )/1100 , // +60 above and -10 because the scissor is outside the 1000 x 1000 draw areea a bit, need to fix this by getting size variable implemented fully
 	 					(float)window_y/2.0 + (float)window_x * 0.033, 
	 	 				(float)window_x * 0.07,
			 			(float)window_x * 0.12 );
 		}
 		
		glEnable( GL_SCISSOR_TEST );
 		
		#if 0// use for testing 
 		glColor3ub( 100, 100, 100 ); // Draw black background
 		glBegin(GL_POLYGON);
 			glVertex2f( -1000, -1000 );
 			glVertex2f( -1000, 1000 );
 			glVertex2f( 1000, 1000 );
 			glVertex2f( 1000, -1000 );
 		glEnd();
		#endif

 		glColor3f( 1.0, 1.0, 1.0 );;

 		// 1 is the most complicated
 		// Middle digit

 		sprintf( string, "%i", spd);

 		int three_one = spd;
 		int five_one = ( three_one - 2 )%10;
 		int four_one = ( three_one - 1 )%10;

 		int two_one = ( three_one + 1 + 10 )%10;
 		int one_one = ( three_one + 2 + 10 )%10;

 		if( three_one == 0 ) 
 		{
 			four_one = 9;
 			five_one = 8;
 		}

 		// Figure out the Speed translation factor for the one's place

 		fontHeight = .34 ;

 		glTranslated( 0, (decimal+.3) * 40, 0 );

 		sprintf( string, "%i", one_one );
 		stroke_output( 64, 120, fontHeight, string );

 		sprintf( string, "%i", two_one );
 		stroke_output( 64, 75, fontHeight, string );

 		sprintf( string, "%i", three_one );
 		stroke_output( 64, 30, fontHeight, string );

 		sprintf( string, "%i", four_one );
 		stroke_output( 64, -15, fontHeight, string );

 		sprintf( string, "%i", five_one );
 		stroke_output( 64, -60, fontHeight, string );
 		
 		glColor3ub( 0,255,0); 

		#if 0
	 	glBegin(GL_POLYGON);
 			glVertex2f( -500, -500 );
 			glVertex2f( -500, 800 );
 			glVertex2f( 1000, 800 ); 
 			glVertex2f( 1000, -500 );
 		glEnd();
		#endif

 		glDisable( GL_SCISSOR_TEST );
 		glPopMatrix();
 
 		glPushMatrix();
			glTranslatef( Pos_X, Pos_Y, 0 );
 			glColor3f( 1.0, 1.0, 1.0 );
 			glLineWidth(2.0);

 			glBegin(GL_LINE_LOOP);
 			 	glVertex2f( size* 0.0, size* 0.0);
 				glVertex2f( size* 0.0, size* 18.0);
 				glVertex2f( size* 22.0, size* 18.0);
 				glVertex2f( size* 22.0, size* 11.0);
 				glVertex2f( size* 25.0, size* 9.0);
 				glVertex2f( size* 22.0, size* 7.0);
 				glVertex2f( size* 22.0, size* 0.0);
 			glEnd();
 		glPopMatrix();
 	
}











