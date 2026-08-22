
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>


void  stroke_output(GLfloat x, GLfloat y, GLfloat size, const char *text );


void PFD_AltTape( int Pos_X, int Pos_Y, float ALT )
{
float size = 5.0;

char  string[50];
//int   m_PhysicalSize_x = size*  34;    
int   height = size * 136;
int   width  = size *  34;
int   alt;
//altitude;

float i = 0; 
float vertOffset;
float tickLocation; 
float tickSpacing  =  size*  11.3;
float tickWidth    =  size*   3.7;
float fontHeight;

 glPushMatrix();
   glTranslatef( Pos_X, Pos_Y - height/2.0, 0 );

   // The speed tape doesn't display speeds < 30 or > 999
   if( ALT <     0.0 ) ALT =     0.0 ;
   if( ALT > 99000.0 ) ALT = 99000.0 ;

   alt      =  (int)ALT      ; 
//   altitude =  (int)ALT - 60 ;
   alt      =  ( (int)ALT/200 )*200 - 400;  

   // Draw the background rectangle
   glColor3ub(51,51,76);  // Color gray-blue
   glBegin(GL_POLYGON);
     glVertex2f(          0 , 0.0              );
     glVertex2f(          0 , height );
     glVertex2f(  width  , height );
     glVertex2f(  width  , 0.0              );
   glEnd();

   glColor3f( 1, 1, 1 ); 
   glLineWidth(2.0);
   glBegin(GL_LINES);
     glVertex2f(  -20,  height/2 );
     glVertex2f(    0,  height/2 );        
   glEnd();

   vertOffset = 0;

   glPushMatrix();

     int even_odd = 1;
     vertOffset = 2.0 * ( ALT - (float)(  ((int)ALT/200 )*200 ) )/ 200;
     for ( i = 0; i <= (float)height + 200; i += tickSpacing )
     {
       tickLocation = (i - ( vertOffset * tickSpacing ));
       even_odd = ~even_odd;

       glLineWidth(2.0);
       glBegin(GL_LINES);
          glVertex2f(  tickWidth, tickLocation );
          glVertex2f(          0, tickLocation );        
       glEnd();

       fontHeight = .25;

       if( even_odd == 1)
       {
         sprintf( string, "%i", (int)(alt/200)*200 );
         if( alt >= 10000 )
         {
           stroke_output(  25 +   10, -10 +  tickLocation, fontHeight,  string );
         }

         else if( alt >=  1000 )
         {
           stroke_output(  25 +   40, -10 +  tickLocation, fontHeight,  string );
         }

         else if( alt >= 100 )
         {
           stroke_output(  25 +   65, -10 +  tickLocation, fontHeight,  string );
         }

         else if( alt == 0 )
         {
           stroke_output(  25 + 115, -10 +  tickLocation, fontHeight,  string );
         }

         else
         {
           stroke_output( 25 +   40, -10 +  tickLocation, fontHeight,  string );
         }

         alt = alt + 200;
       }
     }

     glColor3f( 0.0, 0.0, 0.0 );

     glBegin(GL_POLYGON);
       glVertex2f(         -2.0, height );
       glVertex2f(         -2.0, 1000.0 );
       glVertex2f(  width+ 2, 1000.0 );
       glVertex2f(  width+ 2,  height );
     glEnd();

     glBegin(GL_POLYGON);
       glVertex2f(         -2.0, -200 );
       glVertex2f(         -2.0,  0.0 );
       glVertex2f(  width+ 2,  0.0 );
       glVertex2f(  width+ 2, -200 );
     glEnd();

   glPopMatrix();
 glPopMatrix();
}

