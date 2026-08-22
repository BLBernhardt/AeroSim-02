
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h> 
//#include <GL/glut.h>

//----------------------------------------------------------------------------------------
// oder of these 4 items must be GL/freeglut.h after #include <glad/glad.h> #include <GLFW/glfw3.h>
//#include <glad/glad.h> // must be first OpenGL-related header
#include <GLFW/glfw3.h>
//#define FREEGLUT_LIB_PRAGMAS 0 // Tell FreeGLUT not to include the system OpenGL headers
#include <GL/freeglut.h>
//----------------------------------------------------------------------------------------
#include "../7-MATH/Math.h"
#include "../structures.h"
#include "../IPC_CONFIG.h"




void  stroke_output(GLfloat x, GLfloat y, GLfloat size, const char *text );

void Panel( GD_IN_t* GD_IN_t, Display_t* DSPL, Cntrls_t* CTRL )
{
float  Pos_X, Pos_Y;


	Pos_X = -400;
	Pos_Y = -400;
	
	
	glClearColor(0.08f, 0.10f, 0.14f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glPushMatrix();
	 	glTranslatef( Pos_X, Pos_Y, 0);

 // Draw the background rectangle
 		glColor3ub( 51, 51, 76 ); // Color gray-blue
 		glBegin(GL_POLYGON);
 			glVertex2f( 0 , 0.0 );
 			glVertex2f( 0 , 100 );
 			glVertex2f( 300 , 100 );
 			glVertex2f( 300 , 0.0 );
 		glEnd();

	
 		glColor3f( 1.0, 1.0, 1.0 ); // Color gray-blue
           stroke_output(  120, 60, .15,  "ELEV TRIM" );

       glLineWidth( 1.5 );
       glBegin(GL_LINES);
          glVertex2f(    0,  0 );
          glVertex2f(    0, 20 ); 
          glVertex2f(  150,  0 );
          glVertex2f(  150, 20 );           
          glVertex2f(  300,  0 );
          glVertex2f(  300, 20 );  
          glVertex2f(    0,  0 );
          glVertex2f(  300,  0 );                   
       glEnd();

 		glColor3f( 1.0, 1.0, 0.0 ); // Color gray-blue
 	    glTranslatef( 200, 0, 0);
		glBegin(GL_TRIANGLES);
			glVertex2f(  10.0f,  40.0f);
			glVertex2f(  20.0f,  10.0f);
			glVertex2f(  30.0f,  40.0f);
		glEnd();

	glPopMatrix();

			#if 0
// Test code - draw a Blue triangle
		glClearColor(0.08f, 0.10f, 0.14f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glColor3f(0.2f, 0.2f, 1.0f); /* blue */
		glBegin(GL_TRIANGLES);
			glVertex2f( 0.0f, 500.0f);
			glVertex2f(-500.0f, -500.0f);
			glVertex2f( 500.0f, -500.0f);
		glEnd();
#endif
}
