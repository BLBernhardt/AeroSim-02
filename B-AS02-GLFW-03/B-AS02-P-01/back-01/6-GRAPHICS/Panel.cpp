
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
#include "../IPC_UDP_Common.h"



void Panel( TM_Param_t* TM_Tx, Display_t* DSPL, Cntrls_t* CTRL )
{
			#if 1
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
