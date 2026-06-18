#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include <GL/freeglut.h>
#include <time.h>
#include <signal.h>
#include <fcntl.h>
#include <termios.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <sys/time.h>

#include "structures.h"
#include "0-Math/Math.h"
#include "0-Math/Quaternion.h"




#include "0-AeroModel/aero_model.h"
#include "0-AeroModel/coeff_table.h"

AeroModel* aero = NULL;



void Aero_Computations( float, int,  const AeroModel* aero  );


//using namespace FCSim;
//using namespace RCSim::DataClasses;
//FCSim::FlightModel flightmodel;


struct _Cntrls   	Cntrls;
struct _Aerodyn  	Aero;
struct _Frame    	Frame;
struct _Display  	Display;
struct _Discrete 	Disc;
struct _TEST     	test;
struct _TM_Param	TMParam;




 void TM_Print( void );

//===== Glut ====================================

 #define LOOPS_PER_SECOND(x) ((x)/1000000)
 #define PI    3.14159265

 void PFD_Exec( int, int );
 void AeroDynamics( int );
 void flight_dynamics( void );
 void GLUT_Loop_1( );
 void GLUT_Loop_2( );
 void GLUT_Loop_3( );

 int cnt;
 int mode;
 int mouse_pressed;
 int callback_set;
 float mouse_x, mouse_y;

//======  Graphics ==============================

 void 	drawAxis( void );
 void 	drawAirvehicle(void);

 void 	Realtime_Calcs( float ); 

 void  	Flight_Stick( int, int ); 
 void 	draw_2D( );
 void   display( void );
 int    YMAXSCREEN, XMAXSCREEN;     // default 500, see inti in main.c 

//====== Numerical ================================

 void  	 Dynamics_Init( ); 
 Vector3d position;
 Vector3d attitude;
 


Vector3d getAttitude();
Vector3d getPosition();
  
//===== Quaternion code ========================

 GLUquadricObj	*quadric;

 Vector3d     downwards;
 Vector3d     vehicle_position;

 Quaternion4d vehicle_orientation_q;

 Quaternion4d q_new; // radian
 Quaternion4d q_old; // radian
 Quaternion4d q_dlt; // radian
 Vector3d_rate  angl_new; // deg   
 Vector3d_rate  angl_old; // deg
 Vector3d_rate  angl_dlt; // deg  
 Quaternion4d Qtrn_Orient_Disp;
 float vector_norm( Vector3d );
 float vector_dot_product( Vector3d, Vector3d ); 

 Vector3d vector_scale( float, Vector3d  );
 Vector3d vector_add( Vector3d, Vector3d  );
 Vector3d vector_cross_product( Vector3d, Vector3d ); 
 Vector3d quaternion_rotate_vector( Quaternion4d , Vector3d  );
 Vector3d vector_normalize( Vector3d ); 
 
 Quaternion4d Quat_to_Axis( Quaternion4d );

 Quaternion4d make_rotation_quaternion_from_axis_and_angle( Vector3d, float );
 Quaternion4d quaternion_multiply( Quaternion4d a, Quaternion4d b );
 Quaternion4d quaternion_normalize( Quaternion4d );
 Quaternion4d euler_to_quaternion( double, double, double );
 Quaternion4d euler_to_quaternion( double, double, double );
 Quaternion4d multiply_quaternions( Quaternion4d, Quaternion4d ); 
 void quaternion_to_euler( Quaternion4d, double*, double*, double* );

 void  stroke_output(GLfloat, GLfloat, GLfloat, const char* );


//=======================================================================================
//
//      Write alphanumeric characters to graphics
//
//=======================================================================================


void  stroke_output(GLfloat x, GLfloat y, GLfloat size, const char *text )
{
  const char *p;

  glPushMatrix();
  glTranslatef(x, y, 0);
  glScalef( size, size, size );
  for( p = text; *p; p++)
    glutStrokeCharacter(GLUT_STROKE_ROMAN, *p);
  glPopMatrix();
}


//=======================================================================================
//
//      Time and Interupt Functions
//
//=======================================================================================

volatile sig_atomic_t interrupt_flag = 0; // Use a volatile flag to indicate an interrupt

void timer_handler(int signum)	// This must be declared prior to Timer Initialization to provide timer_handler
{								// Function to handle the timer interrupt (SIGALRM)
    interrupt_flag = 1;			// Set the flag to indicate an interrupt occurred
            //    printf(" interupt flag \n");
}

void Timer_Initialization()  	// Function to initialize the timer and set up the signal handler
{
    struct sigaction sa;	    
    sa.sa_handler = &timer_handler;// Set up the signal handler
    sa.sa_flags = SA_RESTART; 	// Restart interrupted system calls
    sigaction(SIGALRM, &sa, NULL);

    struct itimerval timer;		// Configure the timer to expire after 1 second
    timer.it_value.tv_sec = 1;
    timer.it_value.tv_usec = 0;
    timer.it_interval.tv_sec = 0;
    timer.it_interval.tv_usec = 1000;

    // Start the timer
    if (setitimer(ITIMER_REAL, &timer, NULL) == -1) {
        perror("setitimer");
        exit(EXIT_FAILURE);
    }
}

//=======================================================================================
//      Frame Executive 
//=======================================================================================


void Process_Timing()
{
    static int count_1  = 0;
    static int count_200 = 0;
    static int count_50 = 0;
    float dt;
    static float previous_time, current_time;
    int INIT = 1;
    int RUN = 0;
    
    //  glut_idle_process ( Process_Timming )  executes free wheel at 1 meg per second or more.  
//    if( interrupt_flag == 1 )  // Timer Interupt set for 1000 interupts per second. See Timer Initialization
    {
    	interrupt_flag = 0;
    	count_1++;
     	count_200++;   	
     	count_50++;   	
    	

    	if( count_200 > 10 )  //  100 FPS 
    	{ 
     		count_200 = 0;   	
    		TMParam.frame_cnt++;
    	
    	
    	    current_time = glutGet( GLUT_ELAPSED_TIME  )/1000.0;
    		dt = current_time - previous_time;
    	
  			Realtime_Calcs( dt );
//  			flightmodel.Aero_Computations( dt, RUN );
			Aero_Computations( dt, RUN,  aero );
  			//flightmodel.MoveScene( dt );   
  			TM_Print();
  			
  			previous_time = current_time;
  			
    		//printf(" Frame 100 FPS \n" );
		}		
    	if( count_50 > 20 )  //  50.0 FPS 
    	{ 
    		count_50 = 0;
    		glutSetWindow( Display.Window1 );
    		glutPostRedisplay(  );
    		
     		glutSetWindow( Display.Window2 );
    		glutPostRedisplay(  );   		
    		
    		glutSetWindow( Display.Window3 );
    		glutPostRedisplay(  );  
    		
    		//printf(" Frame 50 FPS \n" );
		}		
		
	
		
		
		
		
	}
}



//=======================================================================================
//
//      Glut Render Functions
//
//=======================================================================================


void quaternion_fill_opengl_rotation_matrix( Quaternion4d q, float *m ) 
{
  Vector3d x,   y,   z;
  Vector3d x_t, y_t, z_t;

  x = ( Vector3d){ 1, 0, 0 };
  y = ( Vector3d){ 0, 1, 0 };
  z = ( Vector3d){ 0, 0, 1 };
  

  x_t = quaternion_rotate_vector( q, x );
  y_t = quaternion_rotate_vector( q, y );
  z_t = quaternion_rotate_vector( q, z );


  m[ 0] = vector_dot_product( x, x_t );
  m[ 1] = vector_dot_product( y, x_t );
  m[ 2] = vector_dot_product( z, x_t );
  m[ 3] = 0;
  
  m[ 4] = vector_dot_product( x, y_t );
  m[ 5] = vector_dot_product( y, y_t );
  m[ 6] = vector_dot_product( z, y_t );
  m[ 7] = 0;
  
  m[ 8] = vector_dot_product( x, z_t );
  m[ 9] = vector_dot_product( y, z_t );
  m[10] = vector_dot_product( z, z_t );
  m[11] = 0;

  m[12] = 0;
  m[13] = 0;
  m[14] = 0;
  m[15] = 1;
}

void Realtime_Calcs( float dt ) 
{
 // float s;

 Vector3d   vehicle_movement;
 Vector3d   rotation_axis;
 Quaternion4d vehicle_orientation_a;
 Quaternion4d rotation;
 float angular_rate_x;
 float angular_rate_y;
 float angular_rate; 
 Vector3d x,   y,   z;
 Vector3d x_t, y_t, z_t;
 int mode = 0;

	// x_t, y_t, z_t support graphics rendering angles
	//=== EDL Code =================================================
	//  x is red    pitch
	//  y is green  yaw
	//  z is yellow roll 	   OpengGL coordinates do not match aircraft coordinates
	
    x = ( Vector3d ){ 1, 0, 0 };
  	y = ( Vector3d ){ 0, 1, 0 };
  	z = ( Vector3d ){ 0, 0, 1 };


  	x_t = quaternion_rotate_vector(  vehicle_orientation_q, x );
  	y_t = quaternion_rotate_vector(  vehicle_orientation_q, y );
  	z_t = quaternion_rotate_vector(  vehicle_orientation_q, z );


//==================== MODES ===================================================

    if( 0 )
    {
    	if( mode == 0 ) printf(" Program Off \n");
		if( mode == 1 ) printf(" Original Qauternion Program Active \n");
		if( mode == 2 ) printf(" EDL-Sim Quaternion4d Program Active \n");
	}

//		position = flightmodel.getPosition();
//		attitude = flightmodel.getAttitude();




		position = getPosition();
		attitude = getAttitude();
    
    	vehicle_orientation_q.w = Qtrn_Orient_Disp.w;
    	vehicle_orientation_q.x = Qtrn_Orient_Disp.x;    	
    	vehicle_orientation_q.y = Qtrn_Orient_Disp.y;    	
    	vehicle_orientation_q.z = Qtrn_Orient_Disp.z;  
    	
        quaternion_to_euler( vehicle_orientation_q, &angl_new.p, &angl_new.y, &angl_new.r );  // Convert updated quaternion back to Euler angles    	 
      	
      	Aero.Pitch_d = angl_new.p ;
      	Aero.Roll_d  = angl_new.r ;	
       	Aero.Yaw_d   = angl_new.y ;		
       	
       	
       	
       	
       	
       	
       	
       	
       	
       	if( 0 )   //=== Test Code
		{
			printf("delta time %.1f ms \n", dt);	
	  		printf("Position: X %6.3f  Y %6.3f  Z %6.3f ",     position.x, position.y, position.z );
	  		printf("Attitude: R %6.3f  P %6.3f  Y %6.3f \n\n", attitude.x, attitude.y, attitude.z );	  		
		}  	
  
    
    if( 0 ) //=== Test Code
    {
    	quaternion_to_euler( q_new, &angl_new.p, &angl_new.y, &angl_new.r );      	
    	printf("Output Angles - Roll: %.2f Pitch %.2f Yaw %.2f deg\n", angl_new.r, angl_new.p, angl_new.y );
    	printf("Quaternion    -   W   %.2f   X   %.2f  Y  %.2f Z %.2f \n", q_new.w, q_new.x, q_new.y, q_new.z );
    	
    	printf(" x %.3f y %.3f z %.3f \n",   x_t.x, y_t.x, z_t.x );
  		printf(" x %.3f y %.3f z %.3f \n",   x_t.y, y_t.y, z_t.y );
  		printf(" x %.3f y %.3f z %.3f \n\n", x_t.z, y_t.z, z_t.z );
	}
}


 
 
//=======================================================================================
//
//	Mouse Functions 	
//
//=======================================================================================

void
mouse_motion( int x, int y )
{
   	/* Output when mouse buttons are down */
	static int cnt = 0;
   
   	cnt++;
   	Cntrls.MouseX = x - Cntrls.MouseStartX;
   	Cntrls.MouseY = y - Cntrls.MouseStartY;
    //printf("MM %d %d %d \n", cnt, Cntrls.MouseX, Cntrls.MouseY );

    if( Cntrls.Stick_Enable )
 	{
 		Cntrls.Stick_X = (float)Cntrls.MouseX ;
  		Cntrls.Stick_Y = (float)Cntrls.MouseY ;
  		Cntrls.Stick_Z = (float)Cntrls.MouseZ ;		
 	}
 	if( !Cntrls.Stick_Enable )
 	{ 

 	}

}

void mouse_button( int button, int state, int x, int y)
{

 	/* only updates when mouse botton is pushed */
 	if (button == GLUT_LEFT_BUTTON) 
 	if (state == GLUT_DOWN)
 	{
 	 	Cntrls.MouseStartX = x;
 		Cntrls.MouseStartY = y; 
 		Cntrls.MouseZ      = 0;
 		
 	    Cntrls.Button_Down = 1;
 	    
 	    
 		Cntrls.MouseX = x - Cntrls.MouseStartX;	
 	 	Cntrls.MouseY = y - Cntrls.MouseStartY;

 		Cntrls.Stick_Enable = !Cntrls.Stick_Enable;
 	  	printf("<><><>< %d\n", Cntrls.Stick_Enable );
 	}
 	
 	if( button == GLUT_MIDDLE_BUTTON )
	{
		if( state == GLUT_DOWN ) 
		{
			Cntrls.MouseZ = 0;
			Cntrls.Stick_Z = (float)Cntrls.MouseZ ;
		}		
	}
}

void MouseWheel( int wheel, int direction, int x, int y )
{

	if( direction > 0 )
	{  
//	printf(" >>>> +Direc %d %d \n", direction, Cntrls.MouseZ );
		Cntrls.MouseZ += 1;
		if( Cntrls.MouseZ > +100 ) Cntrls.MouseZ = +100;
	}
	if( direction < 0 )
	{  
//	printf(" >>>> -Direc %d %d \n", direction, Cntrls.MouseZ );
		Cntrls.MouseZ -= 1;
		if( Cntrls.MouseZ < -100 ) Cntrls.MouseZ = -100;
	}

}

//=== End Mouse Functions =====================================================



void keyboard( unsigned char key, int x, int y)
{

    //printf("key -  %d  \n", key );  //Test Code
    Cntrls.key = key;
 
    switch(key) 
    {
		
		// Exit program
	   	case 27:   //  ascii ESC  235 dec
	  		Cntrls.abort = 1;	
	  		
	  		printf("\n\n\n\n\n\n");
            printf( "Exit Program\n" );  
            printf( "\033[0m" );   
            printf( "\033[?25h" ); 
            system( "stty sane" ); 

		// Course Alpha Adjust
		case '1':  
       		Cntrls.angle_alpha -= 0.01;
       	 	if( Cntrls.angle_alpha < -20.0 ) Cntrls.angle_alpha = -20.0;
	  	break;
       	case '2':  
       		Cntrls.angle_alpha += 0.01;
       	 	if( Cntrls.angle_alpha > + 20.0 ) Cntrls.angle_alpha = +20.0;
	  	break;
	  	
	  	// Fine Alpha Adjust
		case '3':  
       		Cntrls.angle_alpha -= 0.001;
       	 	if( Cntrls.angle_alpha < -20.0 ) Cntrls.angle_alpha = -20.0;
	  	break;
       	case '4':  
       		Cntrls.angle_alpha += 0.001;
       	 	if( Cntrls.angle_alpha > + 20.0 ) Cntrls.angle_alpha = +20.0;
	  	break;
	  	
	  	// Elevator command   Up, Zero, Down 
       	case '0':
       		Cntrls.Elevator_Cmd += 0.02;
       	 	if( Cntrls.Elevator_Cmd > +1.0 ) Cntrls.Elevator_Cmd = +1.0;
	  	break;
       	case '9':
       		Cntrls.Elevator_Cmd = 0.0;
	  	break;	  	
       	case '8':
       		Cntrls.Elevator_Cmd -= 0.02;
       	 	if( Cntrls.Elevator_Cmd < -1.0 ) Cntrls.Elevator_Cmd = -1.0;
	  	break;
	  	
	    // Throttle command   Up, Zero, Down 
	  	case 'e':
       		Cntrls.Throttle_Cmd  += 0.02;
       	 	if( Cntrls.Throttle_Cmd  > +1.0 ) Cntrls.Throttle_Cmd = +1.0;
	  	break;
       	case 'd':
       		Cntrls.Throttle_Cmd  = 0.0;
	  	break;
       	case 'c':
       		Cntrls.Throttle_Cmd  -= 0.02;
       	 	if( Cntrls.Throttle_Cmd   < 0 ) Cntrls.Throttle_Cmd = 0;
	  	break;
	  	
	    // Pitch command   Up, Zero, Down 
	  	case 'P': case 'p':
            Cntrls.Pitch_Cmd += 0.01;
            if( Cntrls.Pitch_Cmd > 1.0 ) Cntrls.Pitch_Cmd = 1.0;	  	
	  	break;	  	
	  	case 'O': case 'o': 
            Cntrls.Pitch_Cmd = 0.0;;
      	break;
	  	case 'I': case 'i':       
            Cntrls.Pitch_Cmd -= 0.01;
            if( Cntrls.Pitch_Cmd < -1.0 ) Cntrls.Pitch_Cmd = -1.0;
      	break;
      	
        // Pitch impulse command   Up, Down 
      	case 'T': case 't': 
            Cntrls.Pitch_impulse = +1;
            printf(" PI + \n");
      	break;
	  	case 'Y': case 'y':       
            Cntrls.Pitch_impulse =  -1;
            printf(" PI - \n");
      	break;

		default:
	    break;
    }


 }





void visibility( int state ) 
{
  if( state == GLUT_VISIBLE && !callback_set )
  {
 //   callback_set = 1;
//    previous_time = glutGet( GLUT_ELAPSED_TIME )/1000.0;
//    frame( -1 );
  }
}

//=======================================================================================
//      Glut Timing & Display Functions
//=======================================================================================

//=======================================================================================
//
//      Glut Initilization 
//
//=======================================================================================

void InitGlut()
{

  	cnt = 0;
  	
	Cntrls.abort = 0;  // set to 1 to abort the prograam	

  	Display.Win1_Width	= 500;
  	Display.Win1_Height	= 500;
  	Display.Win2_Width	= 500;
  	Display.Win2_Height	= 500;  
  	Display.Win3_Width	= 500;
  	Display.Win3_Height	= 500; 

    // Start angles deg
    double pitch = 0.0, yaw = 0.0, roll = 0.0;
    // Delta angles deg 
    double pitch_delta = 0.0, yaw_delta = 10.0, roll_delta = 0.0;
    
    q_new    = { 0.0, 0.0, 0.0, 0.0 }; 	// radian
    q_old    = { 0.0, 0.0, 0.0, 0.0 }; 	// radian
    q_dlt    = { 0.0, 0.0, 0.0, 0.0 }; 	// radian
    angl_new = { 0.0, 0.0, 0.0 }; 		// deg   
    angl_old = { 0.0, 0.0, 0.0 }; 		// deg
    angl_dlt = { 0.0, 0.0, 0.0 }; 		// deg

  	q_old = euler_to_quaternion( angl_old.p, angl_old.y, angl_old.r ); // deg     	// Convert input angles to quaternion // initialize quaternion

  	mouse_pressed = 1;
  	mouse_x = 0;
  	mouse_y = 0;
  	callback_set = 0;
  	downwards = ( Vector3d ){ 0, 0, 1 };
  
  	vehicle_position = ( Vector3d ){ 0, 0, 0 };
  	vehicle_orientation_q = ( Quaternion4d ){ 1, 0, 0, 0 };
  
  	glutInitWindowSize( Display.Win1_Width, Display.Win1_Height );
  	//glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );
  	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_MULTISAMPLE | GLUT_DEPTH  );
  	glutInitWindowPosition( 200, 150 );
  	Display.Window1 = glutCreateWindow( " ED Flight Simulation " );
  	glutDisplayFunc( GLUT_Loop_1 );
   	glutMouseFunc( mouse_button );
    glutMouseWheelFunc( MouseWheel );  
  	glutPassiveMotionFunc( mouse_motion ); 	

   	glutInitWindowSize( Display.Win2_Width, Display.Win2_Height );
 // 	glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );
  	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_MULTISAMPLE | GLUT_DEPTH  );
  	glutInitWindowPosition( 700, 150 );
  	Display.Window2 = glutCreateWindow( " ED Flight Sim PFD " ); 	
   	glutDisplayFunc( GLUT_Loop_2 ); 	
  	glutMouseFunc( mouse_button );
    glutMouseWheelFunc( MouseWheel );  
  	glutPassiveMotionFunc( mouse_motion );
  	glutKeyboardFunc( keyboard );
  	
  	glutInitWindowSize( Display.Win3_Width, Display.Win3_Height );
  //	glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );
  	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_MULTISAMPLE | GLUT_DEPTH  );
  	glutInitWindowPosition( 1200, 150 );
  	Display.Window3 = glutCreateWindow( " EDFS Map  " ); 	
   	glutDisplayFunc( GLUT_Loop_3 ); 	
  	glutMouseFunc( mouse_button );
    glutMouseWheelFunc( MouseWheel );  
  	glutPassiveMotionFunc( mouse_motion );
  	glutKeyboardFunc( keyboard );
  	
 // 	glutVisibilityFunc( visibility );
  
  	glutIdleFunc( Process_Timing );   // Real time frames generated here - yes its confusing. Its GLUT

  	printf("\n");


}




int AeroModel_GlobalInit(void)
{
    static AeroModel aero_m = {0};

    if (!AeroModel_Init(&aero_m)) 
    {
        printf("Failed to initialize AeroModel\n");
        return 0;
    }

    aero = &aero_m;
    AeroModel_PrintSummary(aero);
    printf("AeroModel successfully initialized (global pointer ready).\n");
    return 1;
}





//=======================================================================================++++++++++++++++++++++++++++++
//      Glut Initilization 
//=======================================================================================

int main( int argc, char ** argv ) 
{
//==============================================================================================

//   AeroModel aero = {0};
	TMParam.frame_cnt = 0;

	printf("=====================================================================================================================\n");
    if( !AeroModel_GlobalInit() ){ return 1; }

    double alpha = 5.1;
    double CL = CoeffTable1D_Interpolate(&aero->CL_alpha, alpha);

    printf("At Alpha = %.2f deg → CL = %.4f\n", alpha, CL);

  

  //  AeroModel_Destroy(&aero);

//==============================================================================================

	// Tim Init
	float dt = 0.0;
    Timer_Initialization();

	int INIT = 1;
  	Aero_Computations( 0.0, INIT, aero );


  	glutInit( &argc, argv );
 	InitGlut();  
 	
    printf("Run Main Simulation Program ...  \n"); 	
    glutMainLoop( );
	printf("=====================================================================================================================\n");
  	return 1;
}





