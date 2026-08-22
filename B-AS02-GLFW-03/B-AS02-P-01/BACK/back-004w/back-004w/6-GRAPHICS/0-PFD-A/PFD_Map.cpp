#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include "../structures.h"

extern struct _Aerodyn Aero;

typedef struct
{
	float x;
	float y;	
} Points;

extern void stroke_output(GLfloat, GLfloat, GLfloat, const char* );

void EDL_Map(  )
{
 char string[20];
 int ix, iy, isize;

 float theta, st, ct;
 float start, x, y, end;
 float Heading = 0, Pos_X = 0, Pos_Y = 0, Pos_Z = 0;
 static float x1, x2, y1, y2;
 static Points inertial_pos[1000];
 static int  i = 1, j, k = 0, frame_cnt = 0, init = 1;
 

	if( init == 1 )
	{
		init = 0;
 			for( j = 0; j < 1000; j++ )
 			{
 					inertial_pos[j].x = 0.0;
 				    inertial_pos[j].y = 0.0;		
 			}
	}

	ix = 0;
 	iy = 0;
	isize = 450;

 	if( Heading <   0 ) Heading = Heading + 360;
 	if( Heading <   0 ) Heading = Heading + 360;
 	if( Heading > 360 ) Heading = Heading - 360;
 	if( Heading > 360 ) Heading = Heading - 360;


 	glTranslatef( Pos_X, Pos_Y, 0 );	 	 					  				
  	glColor3f(0.0f, 1.0f, 0.0f);

	frame_cnt++;
		
	glLineWidth(1.0);			
	glBegin(GL_LINES );	
 		for( j = 0; j < 10; j++ )
 		{
 			glVertex3f( inertial_pos[j  ].x, inertial_pos[j  ].y, 0.0 );	
	 	}	
 	glEnd;
 			
	if( frame_cnt > 10 )
	{
		frame_cnt = 0; 
			
 		inertial_pos[0].x = Aero.Position_inertial_X_ft/100.0;
 		inertial_pos[0].y = Aero.Position_inertial_Y_ft/100.0;		

 		for( j = 10; j >= 0; j-- )
 		{ 			
 			inertial_pos[j+1].x = inertial_pos[j].x;
 			inertial_pos[j+1].y = inertial_pos[j].y; 		
 			// 	printf("PM 1]  %d  %.3f   %.3f 	\n", j, 	inertial_pos[j].x, inertial_pos[j].y );		
  		}				
 		printf("===================================\n");
	}
 	glPopMatrix(); //1
}




//EOF
//=======================================================================================================================================================






