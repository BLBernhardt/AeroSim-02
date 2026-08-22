#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h> 

void  PFD_Bezzel() 
{

glPushMatrix();
glHint( GL_POLYGON_SMOOTH_HINT, GL_NICEST );

   glColor3f(0.0f, 0.0f, 0.0f);

    glBegin(GL_POLYGON);
      glVertex2f( -500,  500 );
      glVertex2f( -500, 3000 );
      glVertex2f(  500, 3000 );
      glVertex2f(  500,  500 );
    glEnd();

    glBegin(GL_POLYGON);
      glVertex2f(   500, -3000 );
      glVertex2f(   500,  3000 );
      glVertex2f(  3000,  3000 );
      glVertex2f(  3000, -3000 );
    glEnd();

    glBegin(GL_POLYGON);
      glVertex2f( -500, -3000 );
      glVertex2f( -500,  -500 );
      glVertex2f(  500,  -500 );
      glVertex2f(  500, -3000 );
    glEnd();

    glBegin(GL_POLYGON);
      glVertex2f(  -3000, -3000 );
      glVertex2f(  -3000,  3000 );
      glVertex2f(   -500,  3000 );
      glVertex2f(   -500, -3000 );
    glEnd();

  glPopMatrix();
}
