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

//----------------------------------------------------------------------------------------
// oder of these 4 items must be GL/freeglut.h after #include <glad/glad.h> #include <GLFW/glfw3.h>
//#include <glad/glad.h> // must be first OpenGL-related header
#include <GLFW/glfw3.h>
//#define FREEGLUT_LIB_PRAGMAS 0 // Tell FreeGLUT not to include the system OpenGL headers
#include <GL/freeglut.h>
//----------------------------------------------------------------------------------------

#include "../structures.h"
#include "../7-MATH/Math.h"
#include "../7-MATH/Quaternion.h"
#include "../IPC_UDP_Common.h"

//=== Added for GLUT 

#define LOOPS_PER_SECOND(x) ((x)/1000000)
#define PI    3.14159265

//using namespace FCSim;
//using namespace RCSim::DataClasses;


//===== PFD code ========================

//extern struct Cntrls_t  CTRLg;
//extern struct Aerodyn_t Aero_Dg;
//extern struct Display_t DSPL;




//===== Quaternion code ========================

extern GLUquadricObj	*quadric;
extern Quaternion4 vehicle_orientation_q;

void quaternion_fill_opengl_rotation_matrix( Quaternion4, GLfloat* );
void drawAxis( void );
void drawAirvehicle(void);

void Render_2D_Win1( GLFWwindow* windows[3], Server_Tx_t* TMrx, Display_t* DSPL );
void Render_2D_Win2( GLFWwindow* windows[3], Server_Tx_t TM_Stx, Display_t* DSPL, Cntrls_t* CTRL );
void Render_2D_Win3( GLFWwindow* windows[3], Server_Tx_t TM_Stx, Display_t* DSPL );

void PFD_Exec( int offset_X , int offset_Y, Server_Tx_t* TM, Display_t* DSPL, Cntrls_t* CTRL );
void HSI_Map( Server_Tx_t* TM );
 Vector3 getAttitude();
 Vector3 getPosition();
 void drawAirvehicle(void);

void Flight_Stick( int Pos_X, int Pos_Y, Cntrls_t* Cntrls );


void Render_2D_Win1( GLFWwindow* windows[3], Server_Tx_t* TMrx, Display_t* DSPL ) 
{
    if (!windows[0] || glfwWindowShouldClose(windows[0]))
        return;

    glfwMakeContextCurrent(windows[0]);
 //   Render_2D_Win1( &TMrxg );

	// printf(" Render 2D \n");
//	float		winWidth, winHeight;
	static float pitch = 0.0;
	static float roll  = 0.0;
	static float yaw   = 0.0;

  	DSPL->Win1_Width	= 500;
  	DSPL->Win1_Height	= 500;
  	  	
	glViewport( 0, 0, DSPL->Win1_Width, DSPL->Win1_Height );
  	glMatrixMode( GL_PROJECTION );
  	glLoadIdentity(  );
  	gluPerspective( 60, (float)DSPL->Win1_Width/(float)DSPL->Win1_Height, 5, 100 );
  	glMatrixMode( GL_MODELVIEW );
  	glLoadIdentity(  );
	
	quadric = gluNewQuadric();

  	glEnable( GL_DEPTH_TEST );
  	glEnable( GL_LIGHTING );
  	glEnable( GL_LIGHT0 );
  	glEnable( GL_COLOR_MATERIAL );
  
  	glClearColor( 0, 0, 0, 1 );

//====================================================

 // 	GLfloat rotation[16];
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
//	float pitch =  TMrx->pitch_r ; 
//	float roll  =  TMrx->roll_r ;	
//	float yaw   =  TMrx->heading_r ;

	pitch += 0.1; 
	roll  =  45;	
	yaw   =  180;
	
	pitch = TMrx->Pitch;
	yaw   = TMrx->Yaw;
	roll  = TMrx->Roll;

	glRotatef( pitch , 0, 1, 0 );
	glRotatef( yaw   , 0, 0, 1 );
	glRotatef( roll  , 1, 0, 0 );
	
	
 //   	quaternion_fill_opengl_rotation_matrix( vehicle_orientation_q, rotation );
  //  	glMultMatrixf( rotation );

//===================================================================================
 
    	drawAirvehicle();
    	drawAxis();
	}
  	glPopMatrix();
  	
  	glfwSwapBuffers(windows[0]);

}



void Render_2D_Win2( GLFWwindow* windows[3], Server_Tx_t* TM_Stx, Display_t* DSPL, Cntrls_t* CTRL ) 
{
 int  winWidth, winHeight;
 int  DG_X, DG_Y;
// int Pos_X, Pos_Y;
// float scale;
// int offset_X, offset_Y;
// float  Altitude_feet;


    if (!windows[1] || glfwWindowShouldClose(windows[1]))
        return;

    glfwMakeContextCurrent(windows[1]);
  
  glPushAttrib(GL_ALL_ATTRIB_BITS);

  glDisable( GL_DEPTH_TEST );
  glDisable( GL_LIGHTING   );
  glDisable( GL_TEXTURE_2D );

  glMatrixMode( GL_PROJECTION );
  glPushMatrix();
     glLoadIdentity();

		glfwGetFramebufferSize(glfwGetCurrentContext(),  &winWidth, &winHeight );
//     winWidth  = (float)glutGet( GLUT_WINDOW_WIDTH  );
//     winHeight = (float)glutGet( GLUT_WINDOW_HEIGHT );

     if( winHeight >  winWidth ) 
     {
         DSPL->Grid_X = (int)(1100.0 * ( winHeight/winWidth ));
         DSPL->Grid_Y = 1100;
     }

     if( winHeight <= winWidth ) 
     {
         DSPL->Grid_Y = (int)(1100.0 * ( winWidth/winHeight ));
         DSPL->Grid_X = 1100;
     }

     gluOrtho2D( 0,  DSPL->Grid_Y, 0, DSPL->Grid_X ); 
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

		PFD_Exec( 0, 0, TM_Stx, DSPL, CTRL );  //  Position X and Y relative to center of window. 
	#if 0
	    glClearColor(0.08f, 0.10f, 0.14f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    /* --- Window  drawing code goes here --- */
    glBegin(GL_TRIANGLES);
        glColor3f(0.2f, 0.2f, 1.0f);   /* blue */
        glVertex2f( 0.0f,  50.0f);
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


void Render_2D_Win3( GLFWwindow* windows[3], Server_Tx_t* TM_Stx, Display_t* DSPL  ) 
{
 int  winWidth, winHeight;
 int  DG_X, DG_Y;
// int Pos_X, Pos_Y;
// float scale;
// int offset_X, offset_Y;
// float  Altitude_feet;


    if (!windows[2] || glfwWindowShouldClose(windows[2]))
        return;

    glfwMakeContextCurrent(windows[2]);
  
  glPushAttrib(GL_ALL_ATTRIB_BITS);

  glDisable( GL_DEPTH_TEST );
  glDisable( GL_LIGHTING   );
  glDisable( GL_TEXTURE_2D );

  glMatrixMode( GL_PROJECTION );
  glPushMatrix();
     glLoadIdentity();

		glfwGetFramebufferSize(glfwGetCurrentContext(),  &winWidth, &winHeight );
//     winWidth  = (float)glutGet( GLUT_WINDOW_WIDTH  );
//     winHeight = (float)glutGet( GLUT_WINDOW_HEIGHT );

     if( winHeight >  winWidth ) 
     {
         DSPL->Grid_X = (int)(1100.0 * ( winHeight/winWidth ));
         DSPL->Grid_Y = 1100;
     }

     if( winHeight <= winWidth ) 
     {
         DSPL->Grid_Y = (int)(1100.0 * ( winWidth/winHeight ));
         DSPL->Grid_X = 1100;
     }

     gluOrtho2D( 0,  DSPL->Grid_Y, 0, DSPL->Grid_X ); 
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

//	    HSI_Map( TM_Stx );


#if 0
void GLUT_Loop_1( )
{
//	    printf("\n<<<<<  Win 1 >>>>>\n");
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	Render_2D_Win1();
//	glutPostRedisplay();  now Located in the 50 Hz Process_Timing routing
	glutSwapBuffers();
	if( CTRLg.abort ) 
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
	if( CTRLg.abort ) 
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
	if( CTRLg.abort ) 
	{
		printf("\n<<<<<  ABORT 3 >>>>>\n");
		glutLeaveMainLoop();
	}
}
#endif

//=======================================================================================
//
//      Glut Render Functions
//
//=======================================================================================


void quaternion_fill_opengl_rotation_matrix( Quaternion4 q, float *m ) 
{
  Vector3 x,   y,   z;
  Vector3 x_t, y_t, z_t;

  x = ( Vector3){ 1, 0, 0 };
  y = ( Vector3){ 0, 1, 0 };
  z = ( Vector3){ 0, 0, 1 };
  

  x_t = quaternion_rotate_vector2( q, x );
  y_t = quaternion_rotate_vector2( q, y );
  z_t = quaternion_rotate_vector2( q, z );


  m[ 0] = vector_dot_product( x, x_t );
  m[ 1] = vector_dot_product( y, x_t );
  m[ 2] = vector_dot_product( z, x_t );
  m[ 3] = 0;
  
  m[ 4] = vector_dot_product( x, y_t );
  m[ 5] = vector_dot_product( y, y_t );
  m[ 6] = vector_dot_product( z, y_t );
  m[ 7] = 0;
  
  m[ 8] = vector_dot_product( x, z_t );
  m[ 9] = vector_dot_product( y, z_t );
  m[10] = vector_dot_product( z, z_t );
  m[11] = 0;

  m[12] = 0;
  m[13] = 0;
  m[14] = 0;
  m[15] = 1;
}

#if 0
void Realtime_Calcs( float dt, Aerodyn_t *Aero_Dg ) 
{

 Quaternion4 	q_new; // radian
 Quaternion4 	q_dlt; // radian
 Quaternion4 	q_old; // radian
 Vector3_rate  angl_new; // deg   
 Vector3_rate  angl_old; // deg  
 Vector3_rate  angl_dlt; // deg 
 Vector3 		position;
 Vector3 		attitude;

 Vector3 x,   y,   z;
 Vector3 x_t, y_t, z_t;
 int mode = 0;
int first_pass = 1;


	if( first_pass )
	{
		first_pass = 0;
	    q_new    = { 0.0, 0.0, 0.0, 0.0 }; 	// radian
    	q_old    = { 0.0, 0.0, 0.0, 0.0 }; 	// radian
    	q_dlt    = { 0.0, 0.0, 0.0, 0.0 }; 	// radian
    	angl_new = { 0.0, 0.0, 0.0 }; 		// deg   
   		angl_old = { 0.0, 0.0, 0.0 }; 		// deg
    	angl_dlt = { 0.0, 0.0, 0.0 }; 		// deg
  		q_old = euler_to_quaternion(angl_old.p,angl_old.y,angl_old.r ); // deg     	// Convert input angles to quaternion // initialize quaternion
  //		downwards = ( Vector3 ){ 0, 0, 1 };
  	//	vehicle_position = ( Vector3 ){ 0, 0, 0 };

  		vehicle_orientation_q = ( Quaternion4 ){ 1, 0, 0, 0 };
  	}

	// x_t, y_t, z_t support graphics rendering angles
	//=== EDL Code =================================================
	//  x is red    pitch
	//  y is green  yaw
	//  z is yellow roll 	
	
    x = ( Vector3 ){ 1, 0, 0 };
  	y = ( Vector3 ){ 0, 1, 0 };
  	z = ( Vector3 ){ 0, 0, 1 };


  	x_t = quaternion_rotate_vector2(  vehicle_orientation_q, x );
  	y_t = quaternion_rotate_vector2(  vehicle_orientation_q, y );
  	z_t = quaternion_rotate_vector2(  vehicle_orientation_q, z );


//==================== MODES ===================================================

    if( 0 )
    {
    	if( mode == 0 ) printf(" Program Off \n");
		if( mode == 1 ) printf(" Original Qauternion Program Active \n");
		if( mode == 2 ) printf(" EDL-Sim Quaternion4 Program Active \n");
	}

//		position = flightmodel.getPosition();
//		attitude = flightmodel.getAttitude();
		position = getPosition();
		attitude = getAttitude();
    
    	vehicle_orientation_q.w = Qtrn_Orient_Disp.w;
    	vehicle_orientation_q.x = Qtrn_Orient_Disp.x;    	
    	vehicle_orientation_q.y = Qtrn_Orient_Disp.y;    	
    	vehicle_orientation_q.z = Qtrn_Orient_Disp.z;  
    	
        quaternion_to_euler( vehicle_orientation_q, &angl_new.p, &angl_new.y, &angl_new.r );  // Convert updated quaternion back to Euler angles    	 
      	
      	Aero_Dg->Pitch_d = angl_new.p ;
      	Aero_Dg->Roll_d  = angl_new.r ;	
       	Aero_Dg->Yaw_d   = angl_new.y ;		
       	
       	if( 0 )   //=== Test Code
		{
			printf("delta time %.1f ms \n", dt);	
	  		printf("Position: X %6.3f  Y %6.3f  Z %6.3f ",     position.x, position.y, position.z );
	  		printf("Attitude: R %6.3f  P %6.3f  Y %6.3f \n\n", attitude.x, attitude.y, attitude.z );	  		
		}  	
  
    
    if( 0 ) //=== Test Code
    {
    	quaternion_to_euler( q_new, &angl_new.p, &angl_new.y, &angl_new.r );      	
    	printf("Output Angles - Roll: %.2f Pitch %.2f Yaw %.2f deg\n", angl_new.r, angl_new.p, angl_new.y );
    	printf("Quaternion    -   W   %.2f   X   %.2f  Y  %.2f Z %.2f \n", q_new.w, q_new.x, q_new.y, q_new.z );
    	
    	printf(" x %.3f y %.3f z %.3f \n",   x_t.x, y_t.x, z_t.x );
  		printf(" x %.3f y %.3f z %.3f \n",   x_t.y, y_t.y, z_t.y );
  		printf(" x %.3f y %.3f z %.3f \n\n", x_t.z, y_t.z, z_t.z );
	}
}
#endif

//
//=======================================================================================
