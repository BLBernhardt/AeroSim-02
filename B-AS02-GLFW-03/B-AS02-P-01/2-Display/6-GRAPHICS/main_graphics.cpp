//345678901234567890123456789012345678901234567890123456789012345678901234567890
//==============================================================================
//
//			GRAPHICS Function, Main_Graphics.cpp
//
//==============================================================================

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include <GL/freeglut.h>
#include <time.h>
#include <signal.h>
#include <fcntl.h>
#include <termios.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <sys/time.h>

//----------------------------------------------------------------------------------------
// oder of these 4 items must be GL/freeglut.h after #include <glad/glad.h> #include <GLFW/glfw3.h>
//#include <glad/glad.h> // must be first OpenGL-related header
#include <GLFW/glfw3.h>
//#define FREEGLUT_LIB_PRAGMAS 0 // Tell FreeGLUT not to include the system OpenGL headers
#include <GL/freeglut.h>
//----------------------------------------------------------------------------------------

#include "../structures.h"
#include "CONFIG.h"

#include "../7-MATH/Math.h"

#define LOOPS_PER_SECOND(x) ((x)/1000000)

//===== Quaternion code ========================

GLUquadricObj	*quadric;

void drawAxis( void );
void drawAirvehicle(void);

void Render_2D_Win1( GLFWwindow* windows[3], TM_Param_t* TMrx, Display_t* DSPL );
void Render_2D_Win2( GLFWwindow* windows[3], TM_Param_t TM_Stx, Display_t* DSPL, Cntrls_t* CTRL );
void Render_2D_Win3( GLFWwindow* windows[3], TM_Param_t TM_Stx, Display_t* DSPL );

void PFD_Exec( int offset_X , int offset_Y, TM_Param_t* TM, Display_t* DSPL, Cntrls_t* CTRL );
void HSI_Map( TM_Param_t* TM );

Quaternion4 quaternion_multiply3( Quaternion4 a, Quaternion4 b );
Quaternion4 quaternion_conjugate3( Quaternion4 q );

//--------------------------------------------------------------------------------------------------------------

Vector3 quaternion_rotate_vector3( Quaternion4 q, Vector3 v ) 
{
	Quaternion4 vq ={ .w = 0.0, .x = v.x, .y = v.y, .z = v.z };
	vq = quaternion_multiply3( q, vq );
	vq = quaternion_multiply3( vq, quaternion_conjugate3( q ) );
 
	return ( Vector3 ){ vq.x, vq.y, vq.z };
}

float vector_dot_product3( Vector3 a, Vector3 b ) 
{
	return a.x*b.x + a.y*b.y + a.z*b.z;
}

Quaternion4 quaternion_multiply3( Quaternion4 a, Quaternion4 b ) 
{
	return ( Quaternion4 )	{ a.w * b.w - a.x * b.x - a.y * b.y - a.z * b.z,
							  a.w * b.x + a.x * b.w + a.y * b.z - a.z * b.y,
							  a.w * b.y - a.x * b.z + a.y * b.w + a.z * b.x,
							  a.w * b.z + a.x * b.y - a.y * b.x + a.z * b.w 
							};
}

Quaternion4 quaternion_conjugate3( Quaternion4 q ) 
{
 return ( Quaternion4 ){ q.w, -q.x, -q.y, -q.z };
}


//-----------------------------------------------------------------------------------------------------------------
void Render_2D_Win1( GLFWwindow* windows[3], TM_Param_t* TMrx, Display_t* DSPL ) 
{
 	if (!windows[0] || glfwWindowShouldClose(windows[0])) return;

 	glfwMakeContextCurrent(windows[0]);

	static float pitch = 0.0;
	static float roll = 0.0;
	static float yaw = 0.0;

	DSPL->Win1_Width	= 500;
	DSPL->Win1_Height	= 500;
		
	glViewport( 0, 0, DSPL->Win1_Width, DSPL->Win1_Height );
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity( );
	gluPerspective( 60, (float)DSPL->Win1_Width/(float)DSPL->Win1_Height, 5, 100 );
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity( );
	
	quadric = gluNewQuadric();

	glEnable( GL_DEPTH_TEST );
	glEnable( GL_LIGHTING );
	glEnable( GL_LIGHT0 );
	glEnable( GL_COLOR_MATERIAL );
 
	glClearColor( 0, 0, 0, 1 );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	glLoadIdentity();
	glTranslatef( 0, 0, -20 );
 
	glPushMatrix( ); // background
	{
		glColor3f( 0.3, 0.2, 0.6 );
		glScalef( 10, 10, 10 );
		glBegin( GL_QUADS ); 
		{
			glVertex2f( -1, -1 );
			glVertex2f( +1, -1 );
			glVertex2f( +1, +1 );
			glVertex2f( -1, +1 );
		} 
		glEnd(); 
 	}	
	glPopMatrix();
 
	glPushMatrix(); 
	{
	glTranslatef( 0, 0, 10 ); // center on screen
		glRotatef( 90, 1, 0, 0 ); // make Z axis up/down

		pitch 	= TMrx->Pitch;
		yaw 	= TMrx->Yaw;
		roll 	= TMrx->Roll;

		glRotatef( pitch , 0, 1, 0 );
		glRotatef( yaw , 0, 0, 1 );
		glRotatef( roll , 1, 0, 0 );

		drawAirvehicle();
		drawAxis();
	}
	glPopMatrix();	
	glfwSwapBuffers(windows[0]);
}

void Render_2D_Win2( GLFWwindow* windows[4], TM_Param_t* TM_Stx, Display_t* DSPL, Cntrls_t* CTRL ) 
{
 int winWidth, winHeight;
 int DG_X, DG_Y;

 if (!windows[1] || glfwWindowShouldClose(windows[1])) return;

	glfwMakeContextCurrent(windows[1]);
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glDisable( GL_DEPTH_TEST );
	glDisable( GL_LIGHTING );
	glDisable( GL_TEXTURE_2D );

	glMatrixMode( GL_PROJECTION );
	glPushMatrix();
	glLoadIdentity();

	glfwGetFramebufferSize(glfwGetCurrentContext(), &winWidth, &winHeight );

	if( winHeight > winWidth ) 
	{
		DSPL->Grid_X = (int)(1100.0 * ( winHeight/winWidth ));
		DSPL->Grid_Y = 1100;
	}

	if( winHeight <= winWidth ) 
	{
		DSPL->Grid_Y = (int)(1100.0 * ( winWidth/winHeight ));
		DSPL->Grid_X = 1100;
	}

	gluOrtho2D( 0, DSPL->Grid_Y, 0, DSPL->Grid_X ); 
	glMatrixMode( GL_MODELVIEW );

	glPushMatrix();
		glLoadIdentity();

		DG_X = DSPL->Grid_X;
		DG_Y = DSPL->Grid_Y;

		glTranslatef( DG_Y/2, DG_X/2, 0.0 ); // Center of Display Grid
			glColor3f (1.0, 1.0, 1.0);
			glEnable( GL_BLEND );
			glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
		glEnable( GL_LINE_SMOOTH );

		PFD_Exec( 0, 0, TM_Stx, DSPL, CTRL ); // 0, 0,  Position X and Y relative to center of window. 
#if 0
// Test code - draw a Blue triangle
		glClearColor(0.08f, 0.10f, 0.14f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glColor3f(0.2f, 0.2f, 1.0f); /* blue */
		glBegin(GL_TRIANGLES);
			glVertex2f( 0.0f, 50.0f);
			glVertex2f(-50.0f, -50.0f);
			glVertex2f( 50.0f, -50.0f);
		glEnd();
#endif
 		glDisable(GL_LINE_SMOOTH);
		glDisable(GL_BLEND);
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);

 glPopMatrix();

 glEnable(GL_TEXTURE_2D);
 glEnable(GL_DEPTH_TEST);
 glEnable(GL_LIGHTING);

 glPopAttrib();
 glfwSwapBuffers(windows[1]);
}


void Render_2D_Win3( GLFWwindow* windows[3], TM_Param_t* TM_Stx, Display_t* DSPL ) 
{
 int winWidth, winHeight;
 int DG_X, DG_Y;

 	if (!windows[2] || glfwWindowShouldClose(windows[2])) return;

 	glfwMakeContextCurrent(windows[2]);
 	glPushAttrib(GL_ALL_ATTRIB_BITS);
 	glDisable( GL_DEPTH_TEST );
 	glDisable( GL_LIGHTING );
 	glDisable( GL_TEXTURE_2D );

 	glMatrixMode( GL_PROJECTION );
 	glPushMatrix();
 	glLoadIdentity();
	glfwGetFramebufferSize(glfwGetCurrentContext(), &winWidth, &winHeight );

 	if( winHeight > winWidth ) 
 	{
		DSPL->Grid_X = (int)(1100.0 * ( winHeight/winWidth ));
		DSPL->Grid_Y = 1100;
 	}

 	if( winHeight <= winWidth ) 
 	{
			DSPL->Grid_Y = (int)(1100.0 * ( winWidth/winHeight ));
			DSPL->Grid_X = 1100;
 	}

 	gluOrtho2D( 0, DSPL->Grid_Y, 0, DSPL->Grid_X ); 
 	glMatrixMode( GL_MODELVIEW );

 	glPushMatrix();
		glLoadIdentity();

		DG_X = DSPL->Grid_X;
		DG_Y = DSPL->Grid_Y;

		glTranslatef( DG_Y/2, DG_X/2, 0.0 ); // Center of Display Grid
		glColor3f (1.0, 1.0, 1.0);
		glEnable( GL_BLEND );
		glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
		glEnable( GL_LINE_SMOOTH );
			
		HSI_Map( TM_Stx );
		
		glDisable(GL_LINE_SMOOTH);
		glDisable(GL_BLEND);
		glPopMatrix();
		glMatrixMode(GL_PROJECTION);

 	glPopMatrix();

 	glEnable(GL_TEXTURE_2D);
 	glEnable(GL_DEPTH_TEST);
 	glEnable(GL_LIGHTING);

 	glPopAttrib();
 	glfwSwapBuffers(windows[2]);
}







//
//=======================================================================================
