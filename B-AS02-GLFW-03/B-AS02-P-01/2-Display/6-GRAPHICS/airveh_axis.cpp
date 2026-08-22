
//===================================================================================================
// airveh_axis.cpp

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>       
#include <GL/glut.h>


extern GLUquadricObj	*quadric;



void drawAxis( void )
{

    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glEnable(GL_MULTISAMPLE);

#if 1
 //   glClear(GL_COLOR_BUFFER_BIT);
    
    
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glLineWidth( 2.0 );



 //   glDisable( GL_LIGHTING );


    glColor3f( 1, 0, 0  ); //x axis
    glBegin( GL_LINES  );
       glVertex3f(  -2.2, 0, 0  );
       glVertex3f(   2.2, 0, 0  );
    glEnd(  );
    glColor3f( 1, 1, 1  );
    glBegin( GL_LINES  );
       glVertex3f(  2.2,   0, 0  );
       glVertex3f(  2.5, 0, 0  );
    glEnd(  );
    glColor3f( 0, 0, 1  );
    glBegin( GL_LINES  );
       glVertex3f(  -2.2,   0, 0  );
       glVertex3f(  -2.5, 0, 0  );
    glEnd(  );


    glColor3f( 0, 1, 0  ); // y axis
    glBegin( GL_LINES  );
       glVertex3f(  0, -2.2, 0  );
       glVertex3f(  0,  2.2, 0  );
    glEnd(  );
    glColor3f( 1, 1, 1  );
    glBegin( GL_LINES  );
       glVertex3f(  0, 2.2, 0  );
       glVertex3f(  0, 2.5, 0  );
    glEnd(  );
    glColor3f( 0, 0, 1  );
    glBegin( GL_LINES  );
       glVertex3f(  0,  -2.2, 0  );
       glVertex3f(  0,  -2.5, 0  );
    glEnd(  );


    glColor3f( 1, 1, 0  );
    glBegin( GL_LINES  );
       glVertex3f(  0, 0, -2.2 );
       glVertex3f(  0, 0,  2.2 );
    glEnd(  );
   glColor3f( 1, 1, 1  );
    glBegin( GL_LINES  );
       glVertex3f(  0, 0, 2.2  );
       glVertex3f(  0, 0, 2.5 );
    glEnd(  );
    glColor3f( 0, 0, 1  );
    glBegin( GL_LINES  );
       glVertex3f(  0, 0, -2.2  );
       glVertex3f(  0, 0, -2.5 );
    glEnd();
    glEnable( GL_LIGHTING );
    #endif
}



void drawAirvehicle(void)
{

    GLfloat airvehicleColor[4] = { .8, 0, 0, 1 };

    glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, airvehicleColor );
    glDisable( GL_CULL_FACE );

#if 1
 
    glPushMatrix();
     	//  glRotatef(  90, 0, 1, 0 );
      	// glRotatef(  90, 0, 0, 1 );      
     	//  glRotatef( 180, 1, 0, 0 );   
       	glRotatef( -90, 1, 0, 0 );             
       	glTranslatef( -1.45, 0, 0 );
       	glRotatef( 90, 0, 1, 0 );      
       	// fuselage
       	glColor3f( 1, .5, 0 );
      	gluCylinder( quadric, .2, .2, 3.0, 10, 1 );
		// nose
       	glColor3f( 0.8, 0.8, 0.8 );
       	glTranslatef(0, 0, 3.0 );
       	gluSphere( quadric, .2, 10, 10);
       
       	glTranslatef( 0.0, 0.1, -1.5 );
       	glRotatef( 90, 1, 0, 0 );
       	glBegin(GL_POLYGON);
          	glVertex2f(   2,   .2 );
          	glVertex2f(   2,  -.2 );
          	glVertex2f(  -2,  -.2 );
          	glVertex2f(  -2,   .2 );
       	glEnd();
       	
       	glTranslatef( 0, -1.5, 0 );
       	glBegin(GL_POLYGON);
          	glVertex2f(  -.7,   .15 );
          	glVertex2f(  -.7,  -.15 );
          	glVertex2f(   .7,  -.15 );
          	glVertex2f(   .7,   .15 );
       glEnd();
       glEnable(GL_CULL_FACE);
       glDisable( GL_LIGHTING );
       glLineWidth(3.0);
       glBegin(GL_LINES );
         glVertex2f(   .7,   .2 );
         glVertex2f(  -.7,   .2 );
       glEnd();
       glRotatef( 90,0,1,0 );
       glBegin(GL_LINES );
         glVertex2f(   .7,   .2 );
         glVertex2f(   0,   .2 );
       glEnd();
       glRotatef( -90,0,1,0 );
       glTranslatef( 0, 1.5, 0 );
       glBegin(GL_LINES );
         glVertex2f(   2,   .2 );
         glVertex2f(  -2,   .2 );
       glEnd();
    glPopMatrix();
#endif
}
