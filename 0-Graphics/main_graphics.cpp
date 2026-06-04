//345678901234567890123456789012345678901234567890123456789012345678901234567890
//==============================================================================
//
//       GRAPHICS Function,  Main_Graphics.cpp
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



#include "../structures.h"
#include "../0-Math/Math.h"
#include "../0-Math/Quaternion.h"

//=== Added for GLUT 

#define LOOPS_PER_SECOND(x) ((x)/1000000)
#define PI    3.14159265

//using namespace FCSim;
//using namespace RCSim::DataClasses;


//===== PFD code ========================

extern struct _Cntrls  Cntrls;
extern struct _Aerodyn Aero;
extern struct _Frame   Frame;
extern struct _Display Display;
extern struct _Discrete Disc;
extern struct _TEST test;



//===== Quaternion code ========================

extern GLUquadricObj	*quadric;
extern Quaternion4d vehicle_orientation_q;

void quaternion_fill_opengl_rotation_matrix( Quaternion4d, GLfloat* );
void drawAxis( void );
void drawAirvehicle(void);
void Render_2D_Win1();
void Render_2D_Win2();
void Render_2D_Win3();
void PFD_Exec( int, int );
void  EDL_Map();


void Render_2D_Win1() 
{
	// printf(" Render 2D \n");
	float		winWidth, winHeight;
	glViewport( 0, 0, Display.Win1_Width, Display.Win1_Height );
  	glMatrixMode( GL_PROJECTION );
  	glLoadIdentity(  );
  	gluPerspective( 60, (float)Display.Win1_Width/(float)Display.Win1_Height, 5, 100 );
  	glMatrixMode( GL_MODELVIEW );
  	glLoadIdentity(  );
	
	quadric = gluNewQuadric();

  	glEnable( GL_DEPTH_TEST );
  	glEnable( GL_LIGHTING );
  	glEnable( GL_LIGHT0 );
  	glEnable( GL_COLOR_MATERIAL );
  
  	glClearColor( 0, 0, 0, 1 );

//====================================================

  	GLfloat rotation[16];
  	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

  
  	glLoadIdentity();
  	glTranslatef( 0, 0, -20 );
  
  	glPushMatrix(  );   // background
  	{
    	glColor3f( 0.3, 0.2, 0.6  );
    	glScalef( 10, 10, 10 );
    	glBegin( GL_QUADS  ); 
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

//===================================================================================

    	quaternion_fill_opengl_rotation_matrix( vehicle_orientation_q, rotation );
    	glMultMatrixf( rotation );

//===================================================================================
 
    	drawAirvehicle();
    	drawAxis();
	}
  	glPopMatrix();

//  glutSwapBuffers(  );  // see glut_Loop()
}

void Render_2D_Win2() 
{
 float		winWidth, winHeight;
 int  DG_X, DG_Y;
 int Pos_X, Pos_Y;
 //float scale;
 int offset_X, offset_Y;
 //float  Altitude_feet;


  glPushAttrib(GL_ALL_ATTRIB_BITS);

  glDisable( GL_DEPTH_TEST );
  glDisable( GL_LIGHTING   );
  glDisable( GL_TEXTURE_2D );

  glMatrixMode( GL_PROJECTION );
  glPushMatrix();
     glLoadIdentity();

     winWidth  = (float)glutGet( GLUT_WINDOW_WIDTH  );
     winHeight = (float)glutGet( GLUT_WINDOW_HEIGHT );

     if( winHeight >  winWidth ) 
     {
         Display.Grid_X = (int)(1100.0 * ( winHeight/winWidth ));
         Display.Grid_Y = 1100;
     }

     if( winHeight <= winWidth ) 
     {
         Display.Grid_Y = (int)(1100.0 * ( winWidth/winHeight ));
         Display.Grid_X = 1100;
     }

     gluOrtho2D( 0,  Display.Grid_Y, 0, Display.Grid_X ); 
     glMatrixMode( GL_MODELVIEW );

     glPushMatrix();
        glLoadIdentity();

        DG_X = Display.Grid_X;
        DG_Y = Display.Grid_Y;

	 	glTranslatef( DG_Y/2, DG_X/2, 0.0 ); // Center of Display Grid

        glColor3f (1.0, 1.0, 1.0);

        glEnable( GL_BLEND );
		glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
		glEnable( GL_LINE_SMOOTH );

// goto L500;
// L500:
		PFD_Exec( 0, 0 );  //  Position X and Y relative to center of window. 
		

       glDisable(GL_LINE_SMOOTH);
       glDisable(GL_BLEND);
       glPopMatrix();
       glMatrixMode(GL_PROJECTION);

    glPopMatrix();

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);

    glPopAttrib();
//  glutPostRedisplay();
}


void Render_2D_Win3() 
{
 float		winWidth, winHeight;
 int  DG_X, DG_Y;
 int Pos_X, Pos_Y;
 //float scale;
 int offset_X, offset_Y;
 //float  Altitude_feet;


  glPushAttrib(GL_ALL_ATTRIB_BITS);

  glDisable( GL_DEPTH_TEST );
  glDisable( GL_LIGHTING   );
  glDisable( GL_TEXTURE_2D );

  glMatrixMode( GL_PROJECTION );
  glPushMatrix();
     glLoadIdentity();

     winWidth  = (float)glutGet( GLUT_WINDOW_WIDTH  );
     winHeight = (float)glutGet( GLUT_WINDOW_HEIGHT );

     if( winHeight >  winWidth ) 
     {
         Display.Grid_X = (int)(1100.0 * ( winHeight/winWidth ));
         Display.Grid_Y = 1100;
     }

     if( winHeight <= winWidth ) 
     {
         Display.Grid_Y = (int)(1100.0 * ( winWidth/winHeight ));
         Display.Grid_X = 1100;
     }

     gluOrtho2D( 0,  Display.Grid_Y, 0, Display.Grid_X ); 
     glMatrixMode( GL_MODELVIEW );

     glPushMatrix();
        glLoadIdentity();

        DG_X = Display.Grid_X;
        DG_Y = Display.Grid_Y;

	 	glTranslatef( DG_Y/2, DG_X/2, 0.0 ); // Center of Display Grid

        glColor3f (1.0, 1.0, 1.0);

        glEnable( GL_BLEND );
		glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
		glEnable( GL_LINE_SMOOTH );

// goto L500;
// L500:
	//	PFD_Exec( 0, 0 );  //  Position X and Y relative to center of window. 
		EDL_Map();
		

       glDisable(GL_LINE_SMOOTH);
       glDisable(GL_BLEND);
       glPopMatrix();
       glMatrixMode(GL_PROJECTION);

    glPopMatrix();

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);

    glPopAttrib();
//  glutPostRedisplay();
}

void GLUT_Loop_1( )
{
//	    printf("\n<<<<<  Win 1 >>>>>\n");
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	Render_2D_Win1();
//	glutPostRedisplay();  now Located in the 50 Hz Process_Timing routing
	glutSwapBuffers();
	if( Cntrls.abort ) 
	{
	    printf("\n<<<<<  ABORT 1 >>>>>\n");
		glutLeaveMainLoop();
	}	
}

void GLUT_Loop_2( )
{
//	    printf("\n<<<<<  Win 2 >>>>>\n");
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	Render_2D_Win2();
//	glutPostRedisplay();  now Located in the 50 Hz Process_Timing routing
	glutSwapBuffers();
	if( Cntrls.abort ) 
	{
		printf("\n<<<<<  ABORT 2 >>>>>\n");
		glutLeaveMainLoop();
	}
}

void GLUT_Loop_3( )
{
//	    printf("\n<<<<<  Win 3 >>>>>\n");
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	Render_2D_Win3();
//	glutPostRedisplay();  now Located in the 50 Hz Process_Timing routing
	glutSwapBuffers();
	if( Cntrls.abort ) 
	{
		printf("\n<<<<<  ABORT 3 >>>>>\n");
		glutLeaveMainLoop();
	}
}

//=======================================================================================
//      End Glut Render Functions
//=======================================================================================
 

 
 
 
//=======================================================================================
//
//	Mouse Functions 	
//
//=======================================================================================
