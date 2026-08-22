
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>


void  stroke_output(GLfloat x, GLfloat y, GLfloat size, const char *text );


void PFD_SpeedTape( int Pos_X, int Pos_Y, float IAS )
{

char string[50];
float size = 5.0;

//int   m_PhysicalSize_x = size* 34; 
int   m_PhysicalSize_y = size* 136;
int   indent_x = 135 ;
int   spd, speed;
int   nextHighestIAS; 

float i = 0;  // counter
float vertOffset;
float tickLocation = 0; 
float tickSpacing =  size*  11.3;
float tickWidth   =  size*   3.7;
float fontHeight  =  size*   5.0;

glPushMatrix();
   glTranslatef( Pos_X, Pos_Y, 0 );

   // The speed tape doesn't display speeds < 30 or > 999
   if( IAS <   0.0 )  IAS =   0.0;
   if( IAS > 999.0 )  IAS = 999.0;

   spd   = (int)IAS;
   speed = (int)IAS - 60;

   nextHighestIAS = ( spd/10 ) * 10;
   if ( nextHighestIAS < spd ) nextHighestIAS += 10;

   // Draw the background rectangle

   glColor3ub(51,51,76);  // Color gray-blue

   glBegin(GL_POLYGON);
     glVertex2f(       0.0, 0.0              );
     glVertex2f(       0.0, m_PhysicalSize_y );
     glVertex2f(  indent_x, m_PhysicalSize_y );
     glVertex2f(  indent_x, 0.0              );
   glEnd();

   vertOffset = 0;
   fontHeight = .25;

   glColor3f( 1, 1, 1 );
   glLineWidth(2.0);

   vertOffset = IAS - (float) (((int)IAS/20)*20) ;
   for ( i = 0; i <= (float)m_PhysicalSize_y + 130; i +=  2 * tickSpacing )
   {
     tickLocation = i - vertOffset * ( tickSpacing*2.0/20.0 ) + 9;

     glLineWidth(2.0);
     glBegin(GL_LINES);
       glVertex2f(  indent_x - tickWidth, tickLocation );
       glVertex2f(  indent_x,             tickLocation );        
     glEnd();

     glLineWidth(1.5);
     glBegin(GL_LINES);
       glVertex2f(  indent_x - tickWidth, tickLocation + tickSpacing );
       glVertex2f(  indent_x,             tickLocation + tickSpacing );        
     glEnd();

     spd = (int)((speed/20)*20);

     if( spd >= 1000 )
     {
       sprintf( string, "%i", spd );
       stroke_output( 10, -10 +  tickLocation, fontHeight,  string );
     }


     if( spd >= 100 )
     {
       sprintf( string, "%i", spd );
       stroke_output( 29, -10 +  tickLocation, fontHeight,  string );
     }

     else if( spd >=  20 )
     {
       sprintf( string, "%i", spd );
       stroke_output( 55, -10 +  tickLocation, fontHeight,  string );
     }

     else  
     {
       sprintf( string, "%i", 0 );
       stroke_output( 82, -10 +  tickLocation, fontHeight,  string );
     }
     speed = speed + 20;
   }

   glColor3f( 0.0, 0.0, 0.0 );

   glBegin(GL_POLYGON);
     glVertex2f(       0.0, m_PhysicalSize_y );
     glVertex2f(       0.0, 1000.0 );
     glVertex2f(  indent_x+ 2, 1000.0 );
     glVertex2f(  indent_x+ 2,  m_PhysicalSize_y );
   glEnd();

   glBegin(GL_POLYGON);
     glVertex2f(       0.0, -200 );
     glVertex2f(       0.0, 0.0 );
     glVertex2f(  indent_x+ 2, 0.0 );
     glVertex2f(  indent_x+ 2, -200 );
   glEnd();

 glPopMatrix();
}

