#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include "../structures.h"

//extern struct Aerodyn_t Aero_Dg;

typedef struct
{
	float x;
	float y;	
} Points;

extern void stroke_output(GLfloat, GLfloat, GLfloat, const char* );

void HSI_Map( Aerodyn_t* AERO_D )
{

 int isize;

 float theta, ct, st, ix =0, iy=0;
 float Pos_X = 0, Pos_Y = 0;

 static Points inertial_pos[1000];
 static int i,  j,  frame_cnt = 0, init = 1;
 
	if( init == 1 )
	{
		init = 0;
 		for( j = 0; j < 1000; j++ )
 		{
 			inertial_pos[j].x = 0.0;
 		    inertial_pos[j].y = 0.0;		
 		}
	}

	isize = 450;
		
 	glPushMatrix(); // 1	 
 	  	glTranslatef( Pos_X, Pos_Y, 0 );	 	 			
 	 		  				
  		glColor3f(1.0f, 1.0f, 1.0f);	
  		glLineWidth(1.0);
 		glBegin(GL_LINES);					
  		for (i = 0; i <= 360; i += 45 )
 		{
 			theta = i * M_PI/ 180;
	 		st = sin(theta);
	 		ct = cos(theta);

	 		glVertex2f( ix + st * 0.47f * isize , iy + ct * 0.47f * isize );
	 		glVertex2f( ix + st * 0.50f * isize , iy + ct * 0.5f * isize );
 		}						
 		glEnd(); 			

  		glColor3f(0.0f, 1.0f, 0.0f);
		frame_cnt++;
		
	 	glLineWidth(2.0);			
		glBegin(GL_LINE_STRIP );	
 			for( j = 0; j < 500; j++ )
 			{
 				glVertex3f( inertial_pos[j  ].x, inertial_pos[j  ].y, 0.0 );
 			}	
 		glEnd();

		if( frame_cnt > 20 )
		{
			frame_cnt = 0; 
			
 			inertial_pos[0].x = AERO_D->Position_inertial_X_ft/100.0;
 			inertial_pos[0].y = AERO_D->Position_inertial_Y_ft/100.0;		

 			for( j = 500; j >= 0; j-- )
 			{ 			
 				inertial_pos[j+1].x = inertial_pos[j].x;
 				inertial_pos[j+1].y = inertial_pos[j].y; 		
 				 //	printf("PM 1]  %d  %.3f   %.3f 	\n", j, 	inertial_pos[j].x, inertial_pos[j].y );		
  			}				
 
 			//printf("===================================\n");
		}
 			
 		glPopMatrix(); //1

}


//EOF
//=============================================================================================================================================






