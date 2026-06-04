#define MODULES 1

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GL/glut.h>
#include "../../structures.h"


//float Aero.Pitch_d    = 0;
//float Aero.Roll_d     = 0;
//float Aero.Speed_Knots  = 120;
//float Aero.Alt_Feet     = 0;
//float Aero.Heading_d  = 0;

int Display_Grid_X = 1000;
int Display_Grid_Y = 1000;
int Pos_X, Pos_Y;


extern void PFD_Attitude( int, int, float, float );
extern void PFD_SpeedTicker(  int, int, int, int, int, int, float );
extern void PFD_AltTicker(    int, int, int, int,  int, int, float );
extern void PFD_SpeedTape(    int, int, float );
extern void PFD_AltTape(      int, int, float );
extern void PFD_Bezzel(       int, int );
extern void PFD_Heading(      int, int, float );
extern void  stroke_output(GLfloat, GLfloat, GLfloat, char* );


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



void 
PFD_Display()
{

int  DG_X, DG_Y;
//float scale;
int offset_X, offset_Y;
 float winWidth;
 float winHeight;


 extern struct _Aerodyn Aero;

 glPushAttrib( GL_ALL_ATTRIB_BITS);//0

 glDisable( GL_DEPTH_TEST );
 glDisable( GL_LIGHTING );
 glDisable( GL_TEXTURE_2D );

 glMatrixMode( GL_PROJECTION );
 glPushMatrix();//1
     glLoadIdentity();

     //    gluOrtho2D(0, 2660, 0, 2000);

     // gluOrtho2D(0, 2000, 0, 2000);


     winWidth  = (float)glutGet( GLUT_WINDOW_WIDTH  );
     winHeight = (float)glutGet( GLUT_WINDOW_HEIGHT );

     if( winHeight >  winWidth ) 
     {
         Display_Grid_X = (int)(1000.0 * ( winHeight/winWidth ));
         Display_Grid_Y = 1000;
     }

     if( winHeight <= winWidth ) 
     {
         Display_Grid_Y = (int)(1000.0 * ( winWidth/winHeight ));
         Display_Grid_X = 1000;
     }

     gluOrtho2D( 0,  Display_Grid_Y, 0, Display_Grid_X ); 

     glMatrixMode( GL_MODELVIEW );

     glPushMatrix();//2
        glLoadIdentity();

        DG_X = Display_Grid_X;
        DG_Y = Display_Grid_Y;

        offset_X =  -68; 
        offset_Y =  100;

	glTranslatef( DG_Y/2, DG_X/2, 0.0 ); // Center of Display Grid

        glColor3f (1.0, 1.0, 1.0);

/* Show the cockpit outline */

 glEnable(GL_BLEND);
 glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
 glEnable(GL_LINE_SMOOTH);
 glLineWidth(1.0);
        glEnable(GL_BLEND);
        glEnable(GL_POLYGON_SMOOTH);

  glFrontFace(GL_CW);

  //        offset_X =  winWidth/2;
  //       offset_Y =  winHeight/2;

    	glEnable( GL_BLEND );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
	glEnable( GL_LINE_SMOOTH );

//	scale = 1;

#if 0
struct                
{
  float  Latitude;   
  float  Longitude; 
  float Position_X_meters;   
  float Position_Z_meters; 
  float Altitude_feet;
  float Altitude_meters;
  float Pitch_d;
  float Yaw_d;
  float Roll_d;
  float Heading_d;
  float Airspeed_meters_per_sec;
}Aero;
#endif





        Pos_X = 0;  Pos_Y = 0;
	//	Background( offset_X,  offset_Y );

        Pos_X = 0 + offset_X;  Pos_Y = 0 + offset_Y;
	       PFD_Attitude( Pos_X, Pos_Y, Aero.Pitch_d, Aero.Roll_d );

	Pos_X = 320 + offset_X;  Pos_Y = -353 + offset_Y;
		PFD_AltTape(  Pos_X, Pos_Y,  Aero.Altitude_MSL_feet );  

	Pos_X = 275 + offset_X;  Pos_Y = -56 + offset_Y;
		PFD_AltTicker(   DG_X, DG_Y,  Pos_X, Pos_Y, offset_X, offset_Y, Aero.Altitude_MSL_feet );  

	Pos_X = -390 + offset_X;  Pos_Y = -350 + offset_Y;
		PFD_SpeedTape(  Pos_X, Pos_Y, Aero.AirSpeed_knots  ); 

	Pos_X = -390 + offset_X;  Pos_Y = -48 + offset_Y;
		PFD_SpeedTicker(  DG_X, DG_Y, Pos_X, Pos_Y, offset_X, offset_Y, Aero.AirSpeed_knots );

	Pos_X = 0 + offset_X;  Pos_Y = -520 + offset_Y;
		PFD_Heading( Pos_X, Pos_Y, Aero.Heading_d );

		//  	PFD_Bezzel( Pos_X, Pos_Y );


  glFrontFace(GL_CCW);


  glPopMatrix();//2

   glDisable(GL_POLYGON_SMOOTH);
   glDisable(GL_BLEND);
   glMatrixMode(GL_PROJECTION);

   glPopMatrix();//1
   glPopAttrib();//0



}

#ifdef NO
void keyboard1(unsigned char key )
{
  //    static texturing = 1;

    switch(key) {
        case '8':
 	  Aero.Pitch_d += 1;
	  printf("%f\n", Aero.Pitch_d);	   
	  break;
        case '2':
 	  Aero.Pitch_d -= 1;	   
	  break;
        case '4':
	  Aero.Roll_d += 5;	   
	  break;
        case '6':
	  Aero.Roll_d -= 5;	   
	  break;
        case '7':
          Aero.Speed_Knots  += 3;
          Aero.Heading_d  += 1;

	  break;
        case '1':
          Aero.Speed_Knots  -= 3;
          Aero.Heading_d  -= 1;

	  break;
        case '9':
          Aero.Alt_Feet  += 10;

	  //	  printf(" Alt %f \n", Aero.Alt_Feet );
	  break;
        case '3':
          Aero.Alt_Feet  -= 10;
	  break;

	default:
	    fprintf(stderr, "Push right mouse button for menu\n");
	    break;
    }
 }

#endif




void point1( )
{


   glPushMatrix();
       glColor3f( 1.0f,  1.0f, 1.0f);
       glBegin(GL_POLYGON);
         glVertex2f(-5, -5);
         glVertex2f(-5,  5);
         glVertex2f( 5,  5);
         glVertex2f( 5, -5);
       glEnd(); 
  glPopMatrix();
}
