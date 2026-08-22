
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include <GL/glut.h>

#include "../structures.h"
#include "defines.h"
#include "flight.h"

struct _Frame   Frame;
struct _Cntrls  Cntrls;
struct _Display Display;
struct _Aerodyn Aero;
struct _Discrete Disc;

extern void PFD_Attitude(     int, int, float, float        );
extern void PFD_SpeedTicker(  int, int, int,   int,   float );
extern void PFD_AltTicker(    int, int, int,   int,   float );
extern void PFD_SpeedTape(    int, int, float               );
extern void PFD_AltTape(      int, int, float               );
extern void PFD_Bezzel(       int, int                      );
extern void PFD_Heading(      int, int, float               );
extern void AeroDynamics( int );
extern void flight_dynamics( void );

float		winWidth, winHeight;

void  	Dynamics_Init( );
void  	Flight_Stick( int, int ); 
void 	draw_2D( );
int     YMAXSCREEN, XMAXSCREEN;     // default 500, see inti in main.c 

void  stroke_output(GLfloat x, GLfloat y, GLfloat size, char *text )
{
  char *p;

  glPushMatrix();
  glTranslatef(x, y, 0);
  glScalef( size, size, size );
  for (p = text; *p; p++)
    glutStrokeCharacter(GLUT_STROKE_ROMAN, *p);
  glPopMatrix();
}

void redraw( )
{
   float  time;

//	Plane temp_plane;
//    float temp_ptw[4][3];

   time = glutGet(GLUT_ELAPSED_TIME); /* time in us */
 
   Frame.delta_time = 10;
 
   if(  time - Frame.Frame_Time >=  Frame.delta_time  ) /* mil sec */
   {  
        if( time  - Frame.Frame_Time > 2 * Frame.delta_time )
        { 
          // printf(" frame slip \n");    
        }  

        Frame.Frame_Time+= Frame.delta_time;

		flight_dynamics();
    //    Dynamics_Run();
		//process_queue( temp_plane, ptw[4][3] );
        
//        sleep_time = ((  Frame.Frame_Time +  Frame.delta_time - time ) * 950000.0 );
//        if( sleep_time < 0 ) sleep_time = 0;
//        usleep( sleep_time ); 

 
    }

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    draw_2D();
    glutSwapBuffers();


}

void draw_2D( )
{

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
         Display.Grid_X = (int)(1000.0 * ( winHeight/winWidth ));
         Display.Grid_Y = 1000;
     }

     if( winHeight <= winWidth ) 
     {
         Display.Grid_Y = (int)(1000.0 * ( winWidth/winHeight ));
         Display.Grid_X = 1000;
     }

     gluOrtho2D( 0,  Display.Grid_Y, 0, Display.Grid_X ); 
     glMatrixMode( GL_MODELVIEW );

     glPushMatrix();
        glLoadIdentity();

        DG_X = Display.Grid_X;
        DG_Y = Display.Grid_Y;

        offset_X =  -68; 
        offset_Y =  100;

	 	glTranslatef( DG_Y/2, DG_X/2, 0.0 ); // Center of Display Grid

        glColor3f (1.0, 1.0, 1.0);

        glEnable( GL_BLEND );
		glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
		glEnable( GL_LINE_SMOOTH );

		glPushMatrix();

//	scale = 1;
	//printf("asdfsdaf\n");
    //    Altitude_feet = Aero.Altitude_MSL_meters;
   //     Altitude_feet = 12000.0;

        	Pos_X = 0;  Pos_Y = 0;

// goto L500;
// L500:

        	Pos_X = 0 + offset_X;  Pos_Y = 0 + offset_Y;

			PFD_Attitude( Pos_X, Pos_Y, Aero.Pitch_deg, Aero.Roll_deg );

			Pos_X = 320 + offset_X;  Pos_Y = -353 + offset_Y;
			PFD_AltTape(  Pos_X, Pos_Y,    Aero.Altitude_AGL_feet  );  

			Pos_X = 275 + offset_X;  Pos_Y = -56 + offset_Y;
			PFD_AltTicker(   DG_X, DG_Y,  Pos_X, Pos_Y,   Aero.Altitude_AGL_feet  );  

			Pos_X = -390 + offset_X;  Pos_Y = -350 + offset_Y;
			PFD_SpeedTape(  Pos_X, Pos_Y,  Aero.AirSpeed_knots ); 

			Pos_X = -390 + offset_X;  Pos_Y = -48 + offset_Y;
			PFD_SpeedTicker(  DG_X, DG_Y, Pos_X, Pos_Y,  Aero.AirSpeed_knots );

			Pos_X = 0 + offset_X;  Pos_Y = -520 + offset_Y;
			PFD_Heading( Pos_X, Pos_Y, Aero.Heading_deg );

			PFD_Bezzel( Pos_X, Pos_Y );

			Flight_Stick( 280, -480 );


       		glColor3f( 0.2, 0.2, 0.2 );
      		glLineWidth(1.0);

       		glBegin(GL_LINE_LOOP);
         		glVertex2f( -500, -500);
         		glVertex2f( -500,  500);
         		glVertex2f(  500,  500);
         		glVertex2f(  500, -500);
       		glEnd();

      glPopMatrix();


       glDisable(GL_LINE_SMOOTH);
       glDisable(GL_BLEND);
       glPopMatrix();
       glMatrixMode(GL_PROJECTION);

    glPopMatrix();

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);

    glPopAttrib();
  glutPostRedisplay();
}


void
display_off(void)
{
  float sleep_time, time;

  time = glutGet(GLUT_ELAPSED_TIME)/1000.0; /* time in us */
 
  if(  time >=  Frame.Frame_Time  ) /* s */
  {  
 
   // Process goes here  //

        sleep_time = ((  Frame.Frame_Time +  Frame.delta_time - time ) * 950000.0 );
        if( sleep_time < 0 ) sleep_time = 0;
        usleep( sleep_time ); 

        time = glutGet(GLUT_ELAPSED_TIME)/1000.0; /* time in us */

        printf("%f %f %f .\n", time,   Frame.Frame_Time, sleep_time/1000000  ); 

        if( time  >   Frame.Frame_Time +   Frame.delta_time )
        { 
           printf(" frame slip \n");    
        } 
        Frame.Frame_Time +=  Frame.delta_time;       

  }

//==============================================================
//  if( time > ( Frame_Time + delta_time ))  // this needs work
//  { printf( "frame slip ++\n"); Frame_Time += delta_time; } 
//==============================================================


}
adfasdf

void
mouse_motion( int x, int y )
{
   /* Output when mouse buttons are down */
static int cnt = 0;
   
   cnt++;
   Cntrls.MouseX = x - Cntrls.MouseStartX;
   Cntrls.MouseY = y - Cntrls.MouseStartY;
   printf("MM %d %d %d \n", cnt, Cntrls.MouseX, Cntrls.MouseY );

}



void
mouse_button(int button, int state, int x, int y)
{


 //Cntrls.MouseX = 0;
 //Cntrls.MouseY = 0; 

 /* only updates when mouse botton is pushed */
 if (button == GLUT_LEFT_BUTTON) 
 if (state == GLUT_DOWN)
 {
  	Cntrls.MouseStartX = x;
 	Cntrls.MouseStartY = y; 
    Cntrls.Button_Down = 1;
 	Cntrls.MouseX = x - Cntrls.MouseStartX;	
  	Cntrls.MouseY = y - Cntrls.MouseStartY;

 	Cntrls.Stick_Enable = !Cntrls.Stick_Enable;
 	  	printf("<><><>< %d\n", Cntrls.Stick_Enable );
 }

}



void
mouse_button_off(int button, int state, int x, int y)
{
  //Cntrls.MouseX = 0;
  //Cntrls.MouseY = 0;  

  /* Updates when mouse botton transition DOWN and UP  */
  if( button == GLUT_LEFT_BUTTON )  
  if( state  == GLUT_DOWN )
  {
       Cntrls.MouseStartX = x;
       Cntrls.MouseStartY = y;  
       Cntrls.Button_Down = 1;
       Cntrls.Stick_Enable = ~Cntrls.Stick_Enable;
   printf(" BD %d %d \n", Cntrls.MouseStartX, Cntrls.MouseStartY );
  }

  if( button == GLUT_LEFT_BUTTON )  
  if( state  == GLUT_UP )
  { 
    Cntrls.Button_Down = 0;
  }


  //  printf(" %d \n", Cntrls.Button_Down );


}

void reshape(int width, int height)
{
    glViewport(0, 0, width, height);
    winWidth = width;
    winHeight = height;
  //  resizeBuffers();
    glutPostRedisplay();
}


void keyboard(unsigned char key, int x, int y)
{

 key = key - 48;
 printf("key -  %d  \n", key );
    switch(key) 
    {

       case 0:
          Aero.AirSpeed_knots_r  += 0.001;
	  break;
       case 9:
          Aero.AirSpeed_knots_r  -= 0.001;
	  break;
        case 8:
 	  Aero.Pitch_deg_r += .001;	   
	  break;
        case 7:
 	  Aero.Pitch_deg_r -= .001;	   
	  break;
        case 6:
	  Aero.Roll_deg_r += .005;	   
	  break;
        case 5:
	  Aero.Roll_deg_r -= .005;	   
	  break;
        case 4:
          Aero.Heading_deg_r  += .001;
	  break;
        case 3:
          Aero.Heading_deg_r  -= .001;
	  break;
        case 2:
          Aero.Altitude_AGL_feet_r  += 0.01;
	  break;
        case 1:
          Aero.Altitude_AGL_feet_r  -= 0.01;
	  break;
 
	default:
	    fprintf(stderr, "Push right mouse button for menu\n");
	    break;
    }
    printf(" 3:4-Head %5.2f 1:2-Alt   %5.2f 9:0-Speed %5.2f 5:6-Roll %5.2f 7:8-Pitch %5.2f                  \n", 
	     Aero.Heading_deg_r,
	     Aero.Altitude_AGL_feet_r,
	     Aero.AirSpeed_knots_r,
	     Aero.Roll_deg_r,
	     Aero.Pitch_deg_r );

 }



void Graphics_Init( void )
{

   struct _Display Display;

   	Frame.Frames_per_second = 50.0;
   	Frame.delta_time = 1.0/  Frame.Frames_per_second;

    glutInitWindowSize( Display.Win_Width = 500, Display.Win_Height = 500 );
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH | GLUT_MULTISAMPLE | GLUT_ALPHA | GLUT_STENCIL  );
    glutInitDisplayMode(  GLUT_DOUBLE );

    glutInitWindowPosition( 1, 0 );
    glutCreateWindow( " Flight Sim 004 " );

    //glutSpecialFunc ( special_key );

    glutDisplayFunc( redraw );
    glutKeyboardFunc( keyboard );
    glutReshapeFunc( reshape );
    glutMouseFunc(  mouse_button );

    //glutMotionFunc( mouse_motion );
	glutPassiveMotionFunc( mouse_motion );

    Display.Grid_X = 1000;
    Display.Grid_Y = 1000;

    glEnable( GL_AUTO_NORMAL );
    glEnable( GL_NORMALIZE   );
    glFrontFace(GL_CW);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    YMAXSCREEN = Display.Grid_Y;
    XMAXSCREEN = Display.Grid_X;
    
    Cntrls.Stick_Enable = 0;

 //   glEnable(GL_DEPTH_TEST);

//    quadric = gluNewQuadric();

 //   glMatrixMode(GL_PROJECTION);
 //   glFrustum(-.33, .33, -.33, .33, .5, 40);

 //   glMatrixMode(GL_MODELVIEW);
 //   gluLookAt(-4, 10, 6, 0, 0, 0, 0, 1, 0);

 //   glEnable(GL_LIGHTING);
 //   glEnable(GL_LIGHT0);

//    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//    glEnable(GL_NORMALIZE);

    /*
     * only need this to clear stencil and only need to clear stencil
     * when you're looking at it; the algorithm works without it.
     */
 //   glClearStencil(5);

}


int main( int argc, char **argv )
{
 printf("Main 001 %d\n", KEYBD);

    printf("Flight Init\n");
    argc = 1;

    printf("Glut Init\n");
    glutInit( &argc, argv );
 
    printf("Graphics Init\n");
	Graphics_Init();

	printf("Dynamics Init\n");
	Dynamics_Init( );

	glutMainLoop();

    return 0;
}


