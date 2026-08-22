
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>

#include "../../structures.h"
#include "../../IPC_UDP_Common.h"

//----------------------------------------------------------------------------------------
#include <GLFW/glfw3.h>
#include <GL/freeglut.h>
//----------------------------------------------------------------------------------------

//extern struct Aerodyn_t Aero_Dg;

void  stroke_output(GLfloat x, GLfloat y, GLfloat size, const char *text );


void PFD_Attitude( int Pos_X, int Pos_Y, TM_Param_t* TM )
{
float fontHeight = .30;

// int window_x;
// int window_y;

float x, y;
float theta;// st, ct;
float start, end;
float offset;
float Pitch, Roll;

		Pitch = TM->Pitch;
		Roll  = TM->Roll;

	//	printf(" >34>> Roll %f \n", Roll );
//====================================================================================================

 	glPushMatrix();  // 1
		glTranslatef( Pos_X, Pos_Y, 0 );
 	
#if 0
 		glfwGetFramebufferSize(glfwGetCurrentContext(), &window_x, &window_y );

 		if( window_x >= window_y ) 
 		{
 	 	glScissor( 	(float)window_x/2.0 + (float)window_y * ((float)-500 /(float)1100) , 	// winddow size see glutGet(GLUT_WINDOW_WIDTH ) glutGet(GLUT_WINDOW_HEIGHT )
 	 				(float)window_y/2.0 - (float)window_y * ((float) 300 /(float)1100) ,  
 //	 				(float)window_y/2.0 + (float)(window_y * ( (float)-600 )/(float)1000), 	// 1000 is the Display.Grid_X, Display.Grid_Y size controlled by the minor WINDOW axis size. 
	 				(float)window_y * (float)600/(float)1100,
					(float)window_y * (float)800/(float)1100);
		}
 		else
 		{
 	 	glScissor( 	(float)window_x/2.0 + (float)(window_x * ( -300 )/1100), 	// winddow size see glutGet(GLUT_WINDOW_WIDTH ) glutGet(GLUT_WINDOW_HEIGHT )
 	 				(float)window_y/2.0 + (float)(window_x * ( -400 )/1100), 	// 1000 is the Display.Grid_X, Display.Grid_Y size controlled by the minor WINDOW axis size. 
	 				(float)window_x * (float)600/(float)1100,
					(float)window_x * (float)900/(float)1100);
 		}	
#endif


// 		glEnable( GL_SCISSOR_TEST );
 
 	#if 0
		glColor3f( 1.0, 0.0, 0.0);
 		glBegin(GL_POLYGON);
 			glVertex2f( -1000, -1000 );
 			glVertex2f( -1000, 1000 );
 			glVertex2f( 1000, 1000 );
 			glVertex2f( 1000, -1000 );
 		glEnd();
	#endif
 
//=====================================================================================================

  	//  printf(" PX %d PY %d \n", Pos_X, Pos_Y );

	glPushMatrix();//2
  		glColor3f(0.65f, 0.35f, 0.05f);  //Brown lower half
    	glBegin(GL_POLYGON);
      		glVertex2f(-400, -400);
      		glVertex2f(-400,  400);
      		glVertex2f( 400,  400);
      		glVertex2f( 400, -400);
    	glEnd();
	glPopMatrix();//2

	glPushMatrix();//3
  	// 	glTranslatef( (float)Pos_X, (float)Pos_Y, 0 );

	// Rotate based on the Roll Angle

    glColor3f( 1.0, 1.0, 1.0);

    if (  TM->Pitch  < -90.0)
    {
       Pitch   = -180.0 -  TM->Pitch;
       Roll   +=  180.0;
    }
    else if (  TM->Pitch > 90.0)
    {
       Pitch  = 180.0 -  TM->Pitch;
       Roll  -= 180.0;
    }
 
 //	printf(">>>  Ptich %f \n", Roll   );
 
    glRotatef( Roll, 0, 0, 1);
    offset = -Pitch * 10.0;

    glColor3f(0.12f, 0.52f, 0.70f);
    glBegin(GL_LINES);
       	glVertex2f(  -400,  0 + offset );
       	glVertex2f(   400,  0 + offset );
    glEnd();

    glColor3f(0.15f, 0.55f, 0.75f);
    glBegin(GL_POLYGON);
      	glVertex2f(-400,    0 + offset );
      	glVertex2f(-400,  1800 + offset );
      	glVertex2f( 400,  1800 + offset );
      	glVertex2f( 400,    0 + offset );
    glEnd();
                          
  	glColor3f( 0.9, 0.9, 0.9 );        // Draw in white
  	glLineWidth( 1.5 );

  	glColor3f( 0.9, 0.9, 0.9 );

  	glBegin(GL_LINES);
  	for( x = -49; x <= 49; x += 2 )
  	{
    	glVertex2f(  -25,  25 * x + offset );
    	glVertex2f(   25,  25 * x + offset );
  	}
  	glEnd();

  	glLineWidth(1.5);
  	glBegin(GL_LINES);
  	for( x = -23; x <= 23; x = x + 2 )
  	{
    	glVertex2f(  -50,  50 * x + offset );
    	glVertex2f(   50,  50 * x + offset );
  	}
  	glEnd();

  	glLineWidth( 1.5 );
  	glBegin(GL_LINES);
  	for( x = -24; x <= 24; x = x + 2 )
  	{
    	glVertex2f(  -100,  50 * x + offset );
    	glVertex2f(   100,  50 * x + offset );
  	}
  	glEnd();

  	glLineWidth(2.0);
  	fontHeight = .3;

  	stroke_output(  116,  785.0 + offset, fontHeight,  "80" );
  	stroke_output( -164,  785.0 + offset, fontHeight,  "80" );
  	stroke_output(  116,  685.0 + offset, fontHeight,  "70" );
  	stroke_output( -164,  685.0 + offset, fontHeight,  "70" );
  	stroke_output(  116,  585.0 + offset, fontHeight,  "60" );
  	stroke_output( -164,  585.0 + offset, fontHeight,  "60" );
  	stroke_output(  116,  485.0 + offset, fontHeight,  "50" );
  	stroke_output( -164,  485.0 + offset, fontHeight,  "50" );
  	stroke_output(  116,  385.0 + offset, fontHeight,  "40" );
  	stroke_output( -164,  385.0 + offset, fontHeight,  "40" );
  	stroke_output(  116,  285.0 + offset, fontHeight,  "30" );
  	stroke_output( -164,  285.0 + offset, fontHeight,  "30" );
  	stroke_output(  116,  185.0 + offset, fontHeight,  "20" );
  	stroke_output( -164,  185.0 + offset, fontHeight,  "20" );
  	stroke_output(  116,   85.0 + offset, fontHeight,  "10" );
  	stroke_output( -164,   85.0 + offset, fontHeight,  "10" );
  	stroke_output(  116, -115.0 + offset, fontHeight,  "10" );
  	stroke_output( -164, -115.0 + offset, fontHeight,  "10" );
  	stroke_output(  116, -215.0 + offset, fontHeight,  "20" );
  	stroke_output( -164, -215.0 + offset, fontHeight,  "20" );
  	stroke_output(  116, -315.0 + offset, fontHeight,  "30" );
  	stroke_output( -164, -315.0 + offset, fontHeight,  "30" );
  	stroke_output(  116, -415.0 + offset, fontHeight,  "40" );
  	stroke_output( -164, -415.0 + offset, fontHeight,  "40" );
  	stroke_output(  116, -515.0 + offset, fontHeight,  "50" );
  	stroke_output( -164, -515.0 + offset, fontHeight,  "50" );
  	stroke_output(  116, -615.0 + offset, fontHeight,  "60" );
  	stroke_output( -164, -615.0 + offset, fontHeight,  "60" );
  	stroke_output(  116, -715.0 + offset, fontHeight,  "70" );
  	stroke_output( -164, -715.0 + offset, fontHeight,  "70" );
  	stroke_output(  116, -815.0 + offset, fontHeight,  "80" );
  	stroke_output( -164, -815.0 + offset, fontHeight,  "80" );
 	// stroke_output(  110, -915.0 + offset, fontHeight,  "90" );
 	// stroke_output( -170, -915.0 + offset, fontHeight,  "90" );

	if(1)
	{
 		glPushMatrix();//4
  		offset = Pitch * 10.0;
  		glRotatef( 180, 0, 0, 1);
  		glColor3f( 0.9, 0.9, 0.9 );
 
  		stroke_output(  116, -1115.0 + offset, fontHeight,  "70" );
  		stroke_output( -164, -1115.0 + offset, fontHeight,  "70" );
  		stroke_output(  116, -1015.0 + offset, fontHeight,  "80" );
  		stroke_output( -164, -1015.0 + offset, fontHeight,  "80" );

  		stroke_output(  116,  1085.0 + offset, fontHeight,  "70" );
  		stroke_output( -164,  1085.0 + offset, fontHeight,  "70" );
  		stroke_output(  116,  985.0 + offset, fontHeight,  "80" );
  		stroke_output( -164,  985.0 + offset, fontHeight,  "80" );

 		glPopMatrix();//4
	}

	glPopMatrix();//3

//====================================================================================

	if( 1 )
	{ 
      	glEnable (GL_BLEND);
 //     	glEnable (GL_POLYGON_SMOOTH);
        glDisable (GL_DEPTH_TEST);
    //    glPolygonMode(GL_FRONT_AND_BACK,GL_LINE); //really cool effect 
      	glHint( GL_POLYGON_SMOOTH_HINT, GL_NICEST );
	}
	
//-------------------- Attitude Indicator Frame - Black  -------------------------------------------------------------------------------------
//				Line Fans needed to cover gaps between triangle fan triangles. - Yes its anouying and there is probably a better way. 
	glPushMatrix(); //5

    glEnable (GL_POLYGON_SMOOTH);
	glLineWidth(1.0f);
	glColor3f(0.0f, 0.0f, 0.0f);

  	start  = M_PI *  30.0f / 180.0f;
  	end    = M_PI *  93.0f / 180.0f;
	glBegin(GL_TRIANGLE_FAN);
	  	glVertex2f( 240, 290 );
	  	for ( theta = start; theta < end; theta += M_PI / 36 )
	  	{
	     	y =  sin(theta) * 270 ;
	     	x =  cos(theta) * 270 ; 
	     	glVertex2f( x + 6, y + 20 );
      	}
	glEnd();

	glBegin(GL_LINES);
		for (float theta = start; theta < end; theta += M_PI / 36.0f)
		{
		    x = cosf(theta) * 270.0f + 6.0f;
		    y = sinf(theta) * 270.0f + 20.0f;
    		glVertex2f(240.0f, 290.0f);
    		glVertex2f(x, y);
		}
	glEnd();

	glBegin(GL_TRIANGLE_FAN);
 		glColor3f( 0.0, 0.0, 0.0);
  		end    = M_PI *  28.0f / 180.0f;
  		start  = M_PI *  93.0f / 180.0f;	
  		glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
		glVertex2f( -240, 290 );
	  	for( theta = start; theta > end; theta -= M_PI / 36 )
	  	{
	     	y =   sin(theta) * 270 ;
	     	x =  -cos(theta) * 270 ; 
	     	glVertex2f( x - 6, y + 20 );
     	}
	glEnd();
	glBegin(GL_LINES);
		for( theta = start; theta > end; theta -= M_PI / 36 )
		{
			glVertex2f( -240.0f, 290.0f );
	     	y =   sin(theta) * 270 ;
	     	x =  -cos(theta) * 270 ; 
	     	glVertex2f( x - 6, y + 20 );
		}
	glEnd();	
	
	glBegin(GL_TRIANGLE_FAN);
 		glColor3f( 0.0, 0.0, 0.0);
  		start  = M_PI * 210.0f / 180.0f;
  		end    = M_PI * 273.0f / 180.0f;
	  	glVertex2f( -240, -290 );
	  	for ( theta = start; theta <= end; theta += M_PI / 36 )
	  	{
	     	y =  sin(theta) * 270 ;
	     	x =  cos(theta) * 270 ; 
	     	glVertex2f( x - 6, y - 20 );
        }
	glEnd();
	glBegin(GL_LINES);
		for ( theta = start; theta <= end; theta += M_PI / 36 )
		{
	  	    glVertex2f( -240, -290 );
	     	y =  sin(theta) * 270 ;
	     	x =  cos(theta) * 270 ; 
	     	glVertex2f( x - 6, y - 20 );
		}
	glEnd();

	glBegin(GL_TRIANGLE_FAN);
		glColor3f( 0.0, 0.0, 0.0);
  		start  = M_PI * 267.0f / 180.0f;
  		end    = M_PI * 332.0f / 180.0f;
	  	glVertex2f( 240, -290 );
	  	for ( theta = start; theta <= end; theta += M_PI / 40 )
	  	{
	     	y =  sin(theta) * 270 ;
	     	x =  cos(theta) * 270 ; 
	     	glVertex2f( x + 6, y - 20 );
        }
	glEnd();
	glBegin(GL_LINES);
	  	for ( theta = start; theta <= end; theta += M_PI / 40 )
		{
	  		glVertex2f( 240, -290 );
	     	y =  sin(theta) * 270 ;
	     	x =  cos(theta) * 270 ; 
	     	glVertex2f( x + 6, y - 20 );
		}
	glEnd();	
	glDisable (GL_POLYGON_SMOOTH);

    glBegin(GL_POLYGON);
       	glColor3f( 0.0f, 0.0f, 0.0f );  //1234
      	glVertex2i( -800, 290 );
      	glVertex2i( -800, 800 );
      	glVertex2i(  800, 800 );
      	glVertex2i(  800, 290 );
    glEnd();

    glBegin(GL_POLYGON);
   		glColor3f( 0.0f, 0.0f, 0.0f );   
      	glVertex2i(  240, -800 );
      	glVertex2i(  240,  800 );
      	glVertex2i(  800,  800 );
      	glVertex2i(  800, -800 );
    glEnd();

    glBegin(GL_POLYGON);
      	glColor3f( 0.0f, 0.0f, 0.0f );   
      	glVertex2i( -800, -800 );
      	glVertex2i( -800, -290 );
      	glVertex2i(  800, -290 );
      	glVertex2i(  800, -800 );
    glEnd();

    glBegin(GL_POLYGON);
    	glColor3f( 0.0f, 0.0f, 0.0f );     
      	glVertex2i(  -800, -800 );
      	glVertex2i(  -800,  800 );
      	glVertex2i(  -240,  800 );
      	glVertex2i(  -240, -800 );
    glEnd();

   	glColor3f( 0.0f, 0.0f, 0.0f );

	glPopMatrix(); //5

//----------------Bank Indicator Triangle --------------------------

	glPushMatrix();  //7

  	glRotatef( Roll, 0, 0, 1); // Rotate based on the bank

  	glColor3ub(255,255,255);   // Draw in white
  	glLineWidth(2.0);          // Specify line width

  	glBegin(GL_LINE_LOOP); // the bottom rectangle
    	glVertex2f( 5* -4.5, 5* 39.5);
    	glVertex2f( 5*  4.5, 5* 39.5);
    	glVertex2f( 5*  4.5, 5* 41.5);
    	glVertex2f( 5* -4.5, 5* 41.5);
  	glEnd();

  	glBegin(GL_LINE_STRIP); // the top triangle
    	glVertex2f( 5* -4.5, 5*  41.5);
    	glVertex2f( 5*    0, 5*  46  );
    	glVertex2f( 5*  4.5, 5*  41.5);
  	glEnd();

	glPopMatrix();  //7
//--------------End draw bank indicator------------


//----------------Attitude Indicator----------------
	glPushMatrix();  //8

	//  glTranslatef( Pos_X, Pos_Y, 0 );

  	// The center axis indicator
  	// Black background
  	glColor3ub( 100,0,0 );
  	glBegin(GL_POLYGON);
    	glVertex2f( 5*   1.25, 5*   1.25);
    	glVertex2f( 5*   1.25, 5*  -1.25);
    	glVertex2f( 5*  -1.25, 5*  -1.25);
    	glVertex2f( 5*  -1.25, 5*   1.25);
    	glVertex2f( 5*   1.25, 5*   1.25);
  	glEnd();
  	// White lines
  	glColor3ub(255,255,255);
  	glLineWidth(2.0);
  	glBegin(GL_LINE_LOOP);
    	glVertex2f( 5*   1.25, 5*   1.25);
    	glVertex2f( 5*   1.25, 5*  -1.25);
    	glVertex2f( 5*  -1.25, 5*  -1.25);
    	glVertex2f( 5*  -1.25, 5*   1.25);
  	glEnd();

  	// The left part
  	// Black background

  	glColor3f( 0, 0, 0 );

  	glBegin(GL_POLYGON);
    	glVertex2f( 5*  -30, 5*  1.25);
    	glVertex2f( 5*  -10, 5*  1.25);
    	glVertex2f( 5*  -10, 5* -1.25);
    	glVertex2f( 5*  -30, 5* -1.25);
    	glVertex2f( 5*  -30, 5*  1.25);
  	glEnd();

  	glBegin(GL_POLYGON);
    	glVertex2f( 5* -10, 5*  1.25);
    	glVertex2f( 5* -10, 5* -5.75);
    	glVertex2f( 5* -13, 5* -5.75);
   	 	glVertex2f( 5* -13, 5*  1.25);
    	glVertex2f( 5* -10, 5*  1.25);
  	glEnd();
  
  	// White lines
  	glColor3ub(255,255,255);
  	glLineWidth(2.0);
  	glBegin(GL_LINE_LOOP);
    	glVertex2f( 5* -30, 5*  1.25);
    	glVertex2f( 5* -10, 5*  1.25);
    	glVertex2f( 5* -10, 5* -5.75);
    	glVertex2f( 5* -13, 5* -5.75);
    	glVertex2f( 5* -13, 5* -1.25);
    	glVertex2f( 5* -30, 5* -1.25);
  	glEnd();

  	// The right part
  	// Black background
  	glColor3ub(0,0,0);

  	glBegin(GL_POLYGON);
    	glVertex2f( 5*  30, 5* -1.25);
    	glVertex2f( 5*  10, 5* -1.25);
    	glVertex2f( 5*  10, 5*  1.25);
    	glVertex2f( 5*  30, 5*  1.25);
  	glEnd();

  	glBegin(GL_POLYGON);
    	glVertex2f( 5*  10, 5* -5.75);
    	glVertex2f( 5*  10, 5*  1.25);
    	glVertex2f( 5*  13, 5*  1.25);
    	glVertex2f( 5*  13, 5* -5.75);
  	glEnd();
  
  	glColor3ub(255,255,255);  // White lines
  	glLineWidth(2.0);

  	glBegin(GL_LINE_LOOP);
  	  	glVertex2f( 5*  30, 5*  1.25);
    	glVertex2f( 5*  10, 5*  1.25);
    	glVertex2f( 5*  10, 5* -5.75);
    	glVertex2f( 5*  13, 5* -5.75);
    	glVertex2f( 5*  13, 5* -1.25);
    	glVertex2f( 5*  30, 5* -1.25);
  	glEnd();
//--------------End draw attitude indicator------------

//----------------Flight Path Vector -----=------------
int   segments = 10;
float	  radius = 20.0;
//float Pa, Pb;

//	Pa = TM->Alpha  * 10.0;
//	Pb = TM->Beta   * 10.0;

 //   glTranslatef( Pb, Pa, 0 );

    glBegin(GL_LINE_LOOP);  // Use GL_POLYGON for a circle
    for (int i = 0; i < segments; i++) 
    {
        float theta = 2.0f * M_PI * i / segments;
        float x = radius * cosf(theta);
        float y = radius * sinf(theta);
        glVertex2f( x, y );
    }
    glEnd();

	glPopMatrix(); //8


//----------------Flight Director----------------
	glPushMatrix();  //9

  // Draw the glideslope needles only if the flight director
  // isn't activated and the glideslope is alive

  if ( 0 ) 
  {
    glColor3ub(0,255,0);
    glLineWidth(2.4);

    //	float val = m_pDataSource->Barometric_Alt_Feet;
    float val = 300;
	if (val > 400)
		val = 400;
	//	float tx = m_pDataSource->Nav1_Localizer_Needle * 36;
	float tx = 12;
	float ty = (400 - val) / 400 * 44.5;
    glTranslated(tx, ty, 0);
    glBegin(GL_LINE_LOOP);
      	glVertex2f( 5* -14, 5*    0);
      	glVertex2f( 5*   0, 5*    0);
      	glVertex2f( 5*   0, 5*  4.5);
      	glVertex2f( 5* -12, 5*  4.5);
    glEnd();

    glBegin(GL_LINE_LOOP);
      	glVertex2f( 5* 14, 5*   0);
      	glVertex2f( 5*  0, 5*   0);
      	glVertex2f( 5*  0, 5* 4.5);
      	glVertex2f( 5* 12, 5* 4.5);
    glEnd();

    glPopMatrix();
    
    glColor3ub(255,0,255);
    glLineWidth(3.0);

//    glPushMatrix();
      	glBegin(GL_LINE_STRIP);
      	  	glVertex2f( 5* tx,    0);
        	glVertex2f( 5* tx, 5* ty);
      	glEnd();
  	}
	glPopMatrix();  //9


//-------------- Bank angle markings ---------------

	glPushMatrix(); //10

// glTranslatef( (float)Pos_X, (float)Pos_Y, 0 );
  	glColor3f( 1.0, 1.0, 1.0 );
  	glLineWidth(2.5);

  // Draw the center detent

  	glColor3f( 1.0, 1.0, 1.0 );
  	glRotated( 60.0,  0, 0, 1 );
  	glBegin(GL_LINES);
    	glVertex2f(  0, 5.0* 46.0 );
    	glVertex2f(  0, 5.0* 51.0 );
  	glEnd();
  
  	glRotated(-15.0,0,0,1);
  	glBegin(GL_LINES);
    	glVertex2f(  0, 5.0*  46.0 );
    	glVertex2f(  0, 5.0*  49.0 );
  	glEnd();

  	glRotated(-15.0,0,0,1);
  	glBegin(GL_LINES);
    	glVertex2f(  0, 5.0*  46.0 );
    	glVertex2f(  0, 5.0*  53.0 );
  	glEnd();

  	glRotated(-10.0,0,0,1);
  	glBegin(GL_LINES);
    	glVertex2f(  0, 5.0*  46.9 );
    	glVertex2f(  0, 5.0*  49.0 );
  	glEnd();

  	glRotated(-10.0,0,0,1);

  	glBegin(GL_LINES);
    	glVertex2f(  0, 5.0*  46.0 );
    	glVertex2f(  0, 5.0*  49.0 );
  	glEnd();

  	glRotated(-10.0,0,0,1);

  	glBegin(GL_POLYGON); // the top triangle
    	glVertex2f( 5*  2.5, 5.0*  50.0   );
    	glVertex2f( 5*    0, 5.0*  47.0   );
    	glVertex2f( 5* -2.5, 5.0*  50.0   );
  	glEnd();

  	glRotated(-10.0,0,0,1);
  	glBegin(GL_LINES);
    	glVertex2f(  0, 5.0*  46.0 );
    	glVertex2f(  0, 5.0*  49.0 );
  	glEnd();

  	glRotated(-10.0,0,0,1);
  	glBegin(GL_LINES);
    	glVertex2f(  0, 5.0*  46.0 );
    	glVertex2f(  0, 5.0*  49.0 );
  	glEnd();

  	glRotated(-10.0,0,0,1);
  	glBegin(GL_LINES);
    	glVertex2f(  0, 5.0*  46.0 );
    	glVertex2f(  0, 5.0*  53.0 );
  	glEnd();

  	glRotated(-15.0,0,0,1);
  	glBegin(GL_LINES);
    	glVertex2f(  0, 5.0*  46.0 );
    	glVertex2f(  0, 5.0*  49.0 );
  	glEnd();

  	glRotated(-15.0,0,0,1);
  	glBegin(GL_LINES);
    	glVertex2f(  0, 5.0*  46.0 );
    	glVertex2f(  0, 5.0*  51.0 );
  	glEnd();

	glPopMatrix(); //10
//-------------- Bank angle markings ---------------

//  glDisable( GL_SCISSOR_TEST );

	glPopMatrix(); //1


}

//===============================================================================================================
// EOF






