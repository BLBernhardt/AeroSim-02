


#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include "structures.h"

extern struct _Frame       Frame;
extern struct _Control     Cntr;
extern struct _Display     Dspl;
extern struct _Electrical  Elec;
extern struct _Vehicle     Vehc;




extern int Button_R_Dn;
extern int Button_L_Dn;

void  stroke_output( float, float, float, const char* );

struct _Switch
{ 
 int   isize;
 int   siz_H[20];
 int   siz_V[20];
 int   pos_H[20];
 int   pos_V[20];
 int   leg_siz[20];
 int   leg_pos[20];
 int   lit_FB[20];
 int   ix;
 int   iy;
 int   num_seg;
 char  legend[20][20];
 float color[20][4];
};


#define FRONT 1;
#define BACK  0;

int  Switch_Module( struct _Switch, int, struct _Electrical );





int SW_mode( int state0, struct _Electrical Elec )
{
 struct _Switch SW;

 static int state1;


 SW.ix = 780;
 SW.iy =  40;
 SW.num_seg = 2;

 sprintf( SW.legend[2], "HOVER" );
  SW.siz_H[2]   = 50;
  SW.siz_V[2]   = 25;
  SW.pos_H[2]   =  2;
  SW.pos_V[2]   = 26;
  SW.leg_siz[2] = 13;
  SW.leg_pos[2] =  7;
  SW.lit_FB[2] = BACK;
  if( state1 == 1 || Elec.lamp_test == 1 ) 
     { SW.color[2][0] = 0.60; SW.color[2][1] = 0.90; SW.color[2][2] = 0.60;}
  else                                     
     { SW.color[2][0] = 0.00; SW.color[2][1] = 0.00; SW.color[2][2] = 0.00;} 


  sprintf(  SW.legend[1], "LAND"  );
  SW.siz_H[1]   = 50;
  SW.siz_V[1]   = 23;
  SW.pos_H[1]   =  2;
  SW.pos_V[1]   =  2;
  SW.leg_siz[1] =  13;
  SW.leg_pos[1] =  10;
  SW.lit_FB[1] = BACK;
  if( state1 == 0 || Elec.lamp_test == 1 ) 
     { SW.color[1][0] = 0.90; SW.color[1][1] = 0.70; SW.color[1][2] = 0.00;}
  else                                     
     { SW.color[1][0] = 0.00; SW.color[1][1] = 0.00; SW.color[1][2] = 0.00;}

  state1 = Switch_Module( SW, state1, Elec );

 return state1;
}
  
int SW_auto( int state0, struct _Electrical Elec )
{
 struct _Switch SW;

 int state1;


 SW.ix = 780;
 SW.iy = 100;
 SW.num_seg = 2;

 sprintf( SW.legend[2], "MAN" );
  SW.siz_H[2]    = 50;
  SW.siz_V[2]    = 25;
  SW.pos_H[2]    =  2;
  SW.pos_V[2]    = 26;
  SW.leg_siz[2]  = 13;
  SW.leg_pos[2]  = 13;
  SW.lit_FB[2]   = FRONT;
  if( state0 == 0 || Elec.lamp_test == 1 ) 
     { SW.color[2][0] = 0.7; SW.color[2][1] = 0.7; SW.color[2][2] = 0.7;}
  else                                     
     { SW.color[2][0] = 0.0; SW.color[2][1] = 0.0; SW.color[2][2] = 0.0;}

  sprintf(  SW.legend[1], "AUTO"  );
  SW.siz_H[1]    = 50;
  SW.siz_V[1]    = 23;
  SW.pos_H[1]    =  2;
  SW.pos_V[1]    =  2;
  SW.leg_siz[1]  = 13;
  SW.leg_pos[1]  = 10; 
  SW.lit_FB[1]   = BACK;
  if( state0 == 1 || Elec.lamp_test == 1 ) 
     { SW.color[1][0] = 0.8; SW.color[1][1] = 0.8; SW.color[1][2] = 0.0;}
  else                                     
     { SW.color[1][0] = 0.0; SW.color[1][1] = 0.0; SW.color[1][2] = 0.0;}

  state1 = Switch_Module( SW, state0, Elec );

 return state1;
}


int SW_pwr( int state0, struct _Electrical Elec )
{
 struct _Switch SW;

 static int state1;

//printf(" x = %d y = %d \n",             );


 SW.ix = 780;
 SW.iy = 300;
 SW.num_seg = 3;

 sprintf( SW.legend[3], "PWR" );
  SW.siz_H[3]    = 50;
  SW.siz_V[3]    = 25;
  SW.pos_H[3]    =  2;
  SW.pos_V[3]    = 26;
  SW.leg_siz[3]  = 14;
  SW.leg_pos[3]  = 13;
  SW.lit_FB[3]   = FRONT;
  SW.color[3][0] = 0.90;  SW.color[3][1] = 0.85;  SW.color[3][2] = 0.80;

  sprintf(  SW.legend[2], "OFF"  );
  SW.siz_H[2]    = 24;
  SW.siz_V[2]    = 23;
  SW.pos_H[2]    =  2;
  SW.pos_V[2]    =  2;
  SW.leg_siz[2]  =  10;
  SW.leg_pos[2]  =  5;
  SW.lit_FB[2]   = FRONT;
  if( state1 == 0 || Elec.lamp_test == 1 ) 
     { SW.color[2][0] = 1.0; SW.color[2][1] = 0.0; SW.color[2][2] = 0.0;}
  else                                     
     { SW.color[2][0] = 0.0; SW.color[2][1] = 0.0; SW.color[2][2] = 0.0;}

  sprintf( SW.legend[1], "ON"   );
  SW.siz_H[1]    = 24;
  SW.siz_V[1]    = 23;
  SW.pos_H[1]    = 28;
  SW.pos_V[1]    =  2;
  SW.leg_siz[1]  =  10;
  SW.leg_pos[1]  =  5;
  SW.lit_FB[1]   = FRONT;
  if(  state1 == 1 || Elec.lamp_test == 1 ) 
     { SW.color[1][0] = 0.0; SW.color[1][1] = 1.0; SW.color[1][2] = 0.0;}
  else                                      
     { SW.color[1][0] = 0.0; SW.color[1][1] = 0.0; SW.color[1][2] = 0.0;}
     
     
 //   printf(" L Button down 11 %d \n", Cntr.Button_L_Dn );

  state1 = Switch_Module( SW, state0, Elec );

  return state1;


}
  

int SW_eng( int state0, struct _Electrical Elec )
{
 struct _Switch SW;

 int state1;

 SW.ix = 780;
 SW.iy = 200;
 SW.num_seg = 3;

 sprintf( SW.legend[3], "ENG" );
  SW.siz_H[3]    = 50;
  SW.siz_V[3]    = 25;
  SW.pos_H[3]    =  2;
  SW.pos_V[3]    = 26;
  SW.leg_siz[3]  = 14;
  SW.leg_pos[3]  = 13;
  SW.lit_FB[3]   = FRONT;
  SW.color[3][0] = 0.90;  SW.color[3][1] = 0.85;  SW.color[3][2] = 0.80;

  sprintf(  SW.legend[2], "OFF"  );
  SW.siz_H[2]    = 24;
  SW.siz_V[2]    = 23;
  SW.pos_H[2]    =  2;
  SW.pos_V[2]    =  2;
  SW.leg_siz[2]  =  10;
  SW.leg_pos[2]  =  5;
  SW.lit_FB[2]   = FRONT;
  if( state0 == 0 || Elec.lamp_test == 1 ) 
     { SW.color[2][0] = 1.0; SW.color[2][1] = 0.0; SW.color[2][2] = 0.0;}
  else                                     
     { SW.color[2][0] = 0.0; SW.color[2][1] = 0.0; SW.color[2][2] = 0.0;}

  sprintf( SW.legend[1], "ON"   );
  SW.siz_H[1]    = 24;
  SW.siz_V[1]    = 23;
  SW.pos_H[1]    = 28;
  SW.pos_V[1]    =  2;
  SW.leg_siz[1]  =  10;
  SW.leg_pos[1]  =  5;
  SW.lit_FB[1]   = FRONT;
  if(  state0 == 1 || Elec.lamp_test == 1 ) 
     { SW.color[1][0] = 0.0; SW.color[1][1] = 1.0; SW.color[1][2] = 0.0;}
  else                                      
     { SW.color[1][0] = 0.0; SW.color[1][1] = 0.0; SW.color[1][2] = 0.0;}

  state1 = Switch_Module( SW, state0, Elec );

  return state1;
}
  

int Switch_Module(struct _Switch SW1, int state, struct _Electrical Elec )
{
  int   i;

  // printf(">>%d %d %d \n", state, SW1.ix, SW1.iy );
  // printf(">><<x %d y %d  \n",  Dspl.Ortho_Cursor_X, Dspl.Ortho_Cursor_Y );

 //SW.ix = 700;
 //SW.iy = 200;

  //  printf(" L Button down 12 %d \n", Cntr.Button_L_Dn );

  if( Dspl.Ortho_Cursor_X > SW1.ix &&  
      Dspl.Ortho_Cursor_X < SW1.ix + 55 )
  if( Dspl.Ortho_Cursor_Y > SW1.iy &&  
      Dspl.Ortho_Cursor_Y < SW1.iy + 55 )
  {

  //  printf("BLD-%d\n", Cntr.Button_L_Dn );
    
    if( Cntr.Button_L_Dn == 1 )
    {    
       state = !state;
       printf(" Toggle \n" );

    }
    Cntr.Button_L_Dn = 0; 
  }

 

  glPushMatrix();
  glLineWidth( 1.0 );  
  glColor3f( 0.1, 0.1, 0.1 );
 
    glBegin(GL_POLYGON);
      glVertex2i( SW1.ix     , SW1.iy      );
      glVertex2i( SW1.ix     , SW1.iy + 54 );
      glVertex2i( SW1.ix + 55, SW1.iy + 54 );
      glVertex2i( SW1.ix + 55, SW1.iy      );
    glEnd();

    for( i = 1; i <= SW1.num_seg; i++ )
    {

      if( SW1.lit_FB[i] == 0 ) glColor3f( SW1.color[i][0],  SW1.color[i][1],  SW1.color[i][2] );
      else                     glColor3f( 0.0, 0.0, 0.0 ); 

      glBegin(GL_POLYGON);
        glVertex2f( SW1.ix + SW1.pos_H[i]               , SW1.iy + SW1.pos_V[i]                );
        glVertex2f( SW1.ix + SW1.pos_H[i]               , SW1.iy + SW1.pos_V[i] + SW1.siz_V[i] );
        glVertex2f( SW1.ix + SW1.pos_H[i] + SW1.siz_H[i], SW1.iy + SW1.pos_V[i] + SW1.siz_V[i] );
        glVertex2f( SW1.ix + SW1.pos_H[i] + SW1.siz_H[i], SW1.iy + SW1.pos_V[i]                );
      glEnd();
    
	  //       if( i == 4 )
	  //	  {
	  //	    if( flasher == 1 && disc_crash == 1 ) 
	  //          glColor3f(1.00f, 0.00f, 0.00f);
	  //	  }


      if( SW1.lit_FB[i] == 1 ) glColor3f( SW1.color[i][0],  SW1.color[i][1],  SW1.color[i][2] );
      else                     glColor3f( 0.0, 0.0, 0.0 );      


      stroke_output( SW1.ix + SW1.pos_H[i] + SW1.leg_pos[i], 
                     SW1.iy + SW1.pos_V[i] + SW1.siz_V[i]/3,  
                  (float)SW1.leg_siz[i]/150.0, 
                     SW1.legend[i]  );

      if( Elec.lamp_test == 1 ) Elec.panel_dim = 0.0 ;
      glColor4f( 0.0, 0.0, 0.0, Elec.panel_dim );
      glBegin(GL_POLYGON);
        glVertex2f( SW1.ix + SW1.pos_H[i]               , SW1.iy + SW1.pos_V[i]                );
        glVertex2f( SW1.ix + SW1.pos_H[i]               , SW1.iy + SW1.pos_V[i] + SW1.siz_V[i] );
        glVertex2f( SW1.ix + SW1.pos_H[i] + SW1.siz_H[i], SW1.iy + SW1.pos_V[i] + SW1.siz_V[i] );
        glVertex2f( SW1.ix + SW1.pos_H[i] + SW1.siz_H[i], SW1.iy + SW1.pos_V[i]                );
      glEnd();




    }

    // printf(" %f \n", Elec.panel_dim );

  glPopMatrix();

  return state;
}
