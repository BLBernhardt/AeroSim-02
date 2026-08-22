

//================================================================================================================================================
// main.c

#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <unistd.h>
#include <termios.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <signal.h>
#include <time.h>
#include <sys/time.h>
#include <fcntl.h> 
#include <errno.h> 
#include <stdio.h>
#include <stdlib.h>
#include <sys/select.h>

//----------------------------------------------------------------------------------------
// oder of these 4 items must be GL/freeglut.h after #include <glad/glad.h> #include <GLFW/glfw3.h>
#include <glad/glad.h> // must be first OpenGL-related header
#include <GLFW/glfw3.h>
#define FREEGLUT_LIB_PRAGMAS 0 // Tell FreeGLUT not to include the system OpenGL headers
#include <GL/freeglut.h>
//----------------------------------------------------------------------------------------

#include "keyboard.h"
#include "structures.h"



//=======================================================================================
// Write alphanumeric characters to graphics - GLUT function will replace later
//=======================================================================================
void stroke_output( GLfloat x, GLfloat y, GLfloat size, const char *text )
{
	const char *p;
	glPushMatrix( );
		glTranslatef( x, y, 0 );
		glScalef( size, size, size );
		for( p = text; *p; p++ )
		glutStrokeCharacter( GLUT_STROKE_ROMAN, *p );
	glPopMatrix( );
}






#if 1
#include "structures.h"
#include "CONFIG.h"
#include "GD-A.h"

#include "2-MODEL/aero_model.h"
#include "2-MODEL/coeff_table.h"
#include "4-IPC/Client_TxRx.h"

#include "keyboard.h"

//====== Aero Simulation =============================

	void Aero_Computations( float dt, Aerodyn_t* Aero_D, Cntrls_t* CTRL );

//====== Graphics ==============================

	static void framebuffer_size_callback( GLFWwindow* window, int width, int height ){ glViewport( 0, 0, width, height ); }
	bool Update_Graphics( GLFWwindow* windows[4], GD_t* TMrx, Display_t* DSPL, Cntrls_t* CTRL );
	void Render_2D_Win1( GLFWwindow* windows[4], GD_t* TMrx, Display_t* DSPL );
	void Render_2D_Win2( GLFWwindow* windows[4], GD_t* TMrx, Display_t* DSPL, Cntrls_t* CTRL ); 
	void Render_2D_Win3( GLFWwindow* windows[4], GD_t* TMrx, Display_t* DSPL );
	void Render_2D_Win4( GLFWwindow* windows[4], GD_t* TMrx, Display_t* DSPL, Cntrls_t* CTRL ); 	

 	GLFWwindow* windows[4] = {NULL, NULL, NULL};
	static void error_callback( int error, const char* description ){ fprintf( stderr, "GLFW Error %d: %s\n", error, description ); } 
	int keyboard( Cntrls_t* CTRL );
	void Panel( GD_t* TM_Tx, Display_t* DSPL, Cntrls_t* CTRL );
 bool Update_Graphics( GLFWwindow* windows[4], GD_t* TM_Stx, Display_t* DSPL, Cntrls_t* CTRL );
//======== IPC UDP ===============================

	int tx_sockfd, rx_sockfd;
	struct sockaddr_in tx_addr, rx_addr;
	volatile sig_atomic_t running = 1;

	void handle_sigint( int sig ) 
	{
		( void )sig;
	 	running = 0;
	}

	void UDP_init( );
	void UDP_Tx( TM_Ctx_t* TMtx );
	void UDP_Rx( GSP_UDP_t*  TMrx );

	int  IPC_UDP_C_Rx(int sockfd, GD_t* TMrx);
	void Intf_BBC_GSP( GD_t* TM_Disp, GSP_UDP_t*  GSP, Cntrls_t* CTRL  );
	void Intf_ASD_GD( GD_t* G_Displ,  Aerodyn_t* AERO_D, Cntrls_t* CTRL  );
	void TM_Print( GD_t* GD, TM_Ctx_t* TMrx );	
	void ms_to_hmsms(unsigned long ,unsigned int,unsigned int,unsigned int,unsigned int );

//============ Key board / Mouse =====================================================

	void set_nonblocking_keyboard( void );
	void restore_terminal( void ); // optional, called automatically via atexit
	int getkey( void ); // returns key or -1 if none available
	void key_callback( GLFWwindow* window, int key, int scancode, int action, int mods );


	void cursor_position_callback( GLFWwindow* window, double xpos, double ypos );
	void mouse_button_callback( GLFWwindow* window, int button, int action, int mods );
	void scroll_callback( GLFWwindow* window, double xoffset, double yoffset );

//=======================================================================================
// Time and Interupt Functions
//=======================================================================================

volatile sig_atomic_t interrupt_flag = 0; // Use a volatile flag to indicate an interrupt

void timer_handler( int signum )		// This must be declared prior to Timer Initialization to provide timer_handler
{									// Function to handle the timer interrupt( SIGALRM )
 interrupt_flag = 1;				// Set the flag to indicate an interrupt occurred
}

void Timer_Initialization( )		// Function to initialize the timer and set up the signal handler
{
 struct sigaction sa;	 
	sa.sa_handler = &timer_handler;	// Set up the signal handler
	sa.sa_flags = SA_RESTART;		// Restart interrupted system calls
	sigaction( SIGALRM, &sa, NULL );

 struct itimerval timer;			// Configure the timer to expire after 1 second
	timer.it_value.tv_sec = 1;
	timer.it_value.tv_usec = 0;
	timer.it_interval.tv_sec = 0;
	timer.it_interval.tv_usec = 1000;

	if( setitimer( ITIMER_REAL, &timer, NULL ) == -1 ) // Start the timer
	{
		perror( "setitimer" );
		exit( EXIT_FAILURE );
	}
}

static struct timespec prev_mono = {0};
static int first_call = 1;
static double sim_time_sec = 0.0;

float Time_Update( GD_t *TMtx )
{
 struct timespec ts;

	if( clock_gettime( CLOCK_MONOTONIC_RAW, &ts ) == -1 ) 
	{
		perror( ">>>> error: clock_gettime( CLOCK_MONOTONIC_RAW )" );
		TMtx->dt = 0.0f;
		return 0.0f;
	}

	if( first_call ) 
	{
		time_t real_sec = time( NULL );
		struct tm *tm_info = localtime( &real_sec );
 
		if( tm_info ) 
		{
			TMtx->hours = tm_info->tm_hour;
			TMtx->min = tm_info->tm_min;
			TMtx->sec = tm_info->tm_sec;
			TMtx->mSec =( uint )( ts.tv_nsec / 1000000U );
		} else 
		{
			TMtx->hours = 0;
			TMtx->min = 0;
			TMtx->sec = 0;
			TMtx->mSec = 0;
		}

			sim_time_sec =( double )real_sec +( double )TMtx->mSec / 1000.0;
			first_call = 0;
			prev_mono = ts;
			TMtx->dt = 0.0f;
	} else 
	{
		// High precision monotonic delta
		long long sec_diff = ts.tv_sec - prev_mono.tv_sec;
		long nsec_diff = ts.tv_nsec - prev_mono.tv_nsec;
		TMtx->dt =( float )sec_diff +( float )nsec_diff * 1e-9f;

		sim_time_sec += TMtx->dt;

		time_t sim_sec =( time_t )sim_time_sec;
		struct tm *tm_info = localtime( &sim_sec );

		if( tm_info ) 
		{
			TMtx->hours = tm_info->tm_hour;
			TMtx->min = tm_info->tm_min;
			TMtx->sec = tm_info->tm_sec;
			TMtx->mSec =( uint )(( sim_time_sec -( double )sim_sec ) * 1000.0 );
		} else 
		{
			TMtx->hours = 0;
			TMtx->min = 0;
			TMtx->sec = 0;
			TMtx->mSec = 0;
		}
	}

	prev_mono = ts;
	return TMtx->dt;
}




//=======================================================================================
// Frame Executive 
//=======================================================================================





void Process_Timing( Cntrls_t* CTRL )
{
 static int count_1000	= 0;
 static int count_100	= 0; 
 static int count_40	= 0;
 static int count_20	= 0;
 static int count_10	= 0; 
 
//	static KB_State_t kb				= {0}; 
	static struct Aerodyn_t		ASD		= {0}; 
	static struct Display_t		DSPL	= {0}; 
	static struct TM_Ctx_t 		TM_Ctx	= {0}; 
	static struct GSP_UDP_t     TM_Stx	= {0};	
	static struct GD_t  		GD		= {0};		
	
	unsigned char key;
 
 	if( interrupt_flag == 1 ) // Timer Interupt set for 1000 interupts per second. See Timer Initialization
 	{
		interrupt_flag = 0;
		count_1000++;	
		count_100++;	
		count_40++;
		count_20++; 
		count_10++;		
//-----------------------------------------------------------------------	
// 100 FPS		
		if( count_10 > 10 ) // 100 FPS 
		{ count_10 = 0;	

		}		
//-----------------------------------------------------------------------	
// 50.0 FPS 	
		if( count_20 > 20 ) // 50.0 FPS 
		{ count_20 = 0;
	
			float dt = 0.020; // sec.	
	 		Aero_Computations( dt, &ASD, CTRL );
	 		if( !Update_Graphics( windows, &GD, &DSPL, CTRL )) CTRL->terminate = 1; // all windows closed → exit

	 		TM_Ctx.frame_cnt++;
	//		TM_load( &TM_Stx, &TM_Ctx, &AERO_D, CTRL );
			//UDP_Tx( &TM_Ctx );

			UDP_Rx( &TM_Stx );
		}	
//-----------------------------------------------------------------------	
// 25.0 FPS 	
		if( count_40 > 40 ) 
		{ count_40 = 0;	

		}
//-----------------------------------------------------------------------
// 10.0 FPS 
		if( count_100 > 100 ) 
		{ count_100 = 0;
			//	Time_Update( &TM_Stx );

			Intf_ASD_GD( &GD, &ASD, CTRL  );
			//Intf_BBC_GSP( &TM_Disp, &TM_Stx, CTRL );
	   	 	//TM_Print( &GD, &TM_Ctx );			
			
			if( CTRL->Stick_E ) CTRL->Stick_Y =( float )CTRL->MouseZ;
			else CTRL->MouseZ = 0.0; 
			//	key = keyboard( CTRL );
				
			if( key == 27 ) CTRL->terminate = 1;
			
//		    printf(" roll=%.3f  pitch=%.3f  heading=%.3f   wowN=%u  wowL=%u  wowR=%u\n",
//       				(double)TM_Stx.roll_r,  (double)TM_Stx.pitch_r, (double)TM_Stx.heading_r,
//       						TM_Stx.wowNose,			TM_Stx.wowLeft,			TM_Stx.wowRight);	
		}
//-----------------------------------------------------------------------
// 1.0 FPS 	
		if( count_1000 > 1000 ) // 1.0 FPS 
		{ count_1000 = 0;

			//	printf( ">>>>fame count %d \n", TM_Ctx.frame_cnt );	
		}
//-----------------------------------------------------------------------

	}
}



//==============================================================================
void UDP_init( ) 
{
	signal( SIGINT, handle_sigint );
	IPC_UDP_C_Tx_init( &tx_sockfd, &tx_addr, CLIENT_TX_PORT, DEFAULT_IP );
	IPC_UDP_C_Rx_init( &rx_sockfd, &rx_addr, SERVER_TX_PORT, DEFAULT_IP );
	printf( "Client started Ports: Tx:%d, Rx:%d Press Ctrl+C to stop.\n", CLIENT_TX_PORT, SERVER_TX_PORT );
}

void UDP_Tx( TM_Ctx_t* TMtx )
{
// static uint8_t tx_buf[BUFFER_SIZE]; // or make it a local if you prefer
// size_t pkt_len;

	if( TMtx == NULL ) 
	{
		fprintf( stderr, "UDP_Tx: NULL pointer\n" );
		return;
	}

	// Convert host → network byte order and copy into transmit buffer
//	pkt_len = Prepare_UDP_Msg( TMtx, sizeof( GD_t ), tx_buf, sizeof( tx_buf ));
#if 0
	if( pkt_len == 0 ) 
	{
		fprintf( stderr, "UDP_Tx: Prepare_UDP_Msg failed\n" );
		return;
	}
#endif
	// Non-blocking send( drops packet if send buffer is full )
//	int status = IPC_UDP_C_Tx( tx_sockfd, &tx_addr, tx_buf, pkt_len );

//	if( status < 0 ) 
//	{
		// Real error( optional: add counter or log )
		// fprintf( stderr, "UDP_Tx: send error\n" );
//	}
	// status == 0 → packet dropped because socket would block( normal under load )
	// status == 1 → sent successfully

 //======================================================================================= 
 #if 0
	//packet.sec =( frame / 60 ) % 60;
		//packet.mSec =( frame % 60 ) * 16; // rough ms

	// Fill each byte with its sequential number( 1 .. 62 )
		for( int i = 0; i < BUFFER_SIZE; i++ ) {
		tx_buf[i] =( uint8_t )( i + 1 ); // 01, 02, 03, ... 62
		}
		size_t pkt_len = 62;
	#endif
	//======================================================================================= 
}

void UDP_Rx( GSP_UDP_t* TMrx )
{
	IPC_UDP_C_Rx( rx_sockfd, TMrx );
}

//====================================================================================================
Cntrls_t* g_CTRL = NULL;
GLFWwindow* g_win2 = NULL;



#endif

#if 1 
//========================================================================================
int main( int argc, char ** argv ) 
{
 struct Cntrls_t	CTRL = {0};
 		AeroModel 	aero = {0};
 
//==============================================================================================

	printf( "========================================================================================\n" );
	printf( " AeroSim 02\n" );
	printf( " Cessna 182 Flight Simulation\n" );
	printf( " USe ESC on grphics window to exit program \n" );
	printf( " Needed to reset terminal to normal mode after progrm ends \n" );
//================================================================================================


	if( !AeroModel_Init( &aero )) return 1;
	AeroModel_PrintSummary( &aero );
 

	double alpha = 8.0;
	// double mach = 0.3;
	double CL = 0.0;
	double mach = 1.1;

	CL = CoeffTable2D_Interpolate( &aero.CL_alpha_mach, alpha, mach );
	printf( "2D: At Alpha=%.2f deg, → CL = %.4f\n", alpha, CL );
	CL = CoeffTable1D_Interpolate( &aero.CL_alpha, alpha );
	printf( "1D: At Alpha=%.2f deg, → CL = %.4f\n", alpha, CL );

	// AeroModel_Destroy( &aero );

//==============================================================================================

//	set_nonblocking_keyboard( );
	UDP_init( );
	Timer_Initialization( );
	glutInit( &argc, argv );
	glfwSetErrorCallback( error_callback );






//=====================================================================	
	if( !glfwInit( ) ) 
	{
		fprintf( stderr, "Failed to initialize GLFW\n" );
		return EXIT_FAILURE;
	}
	/* OpenGL 3.0 Compatibility profile( fixed-function still available ), Works with OpenGL 4.6 */
	glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
	glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 0 );
   // glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);	//  this works with OpenGL 3.3 but not OpenGL 3.0
	/* Do NOT set GLFW_OPENGL_CORE_PROFILE */	
	 //   GLFWwindow* windows[4] = {NULL};

	const char* titles[4] = 
	{
		"Window 1 - 6DOF",
		"Window 2 - PFD",
		"Window 3 - Course Map",
		"Window 4 - Controls",
	};


	for( int i = 0; i <= 3; ++i ) 
	{
		windows[i] = glfwCreateWindow( 450, 450, titles[i], NULL, NULL );
		if( !windows[i] ) 
		{
			fprintf( stderr, "Failed to create GLFW window %d\n", i + 1 );
			for( int j = 0; j < i; ++j ) glfwDestroyWindow( windows[j] );
			glfwTerminate( );
			return EXIT_FAILURE;
		}
				glfwSetFramebufferSizeCallback( windows[i], framebuffer_size_callback ); 
		glfwSetWindowUserPointer( windows[i], &CTRL );
		glfwSetCursorPosCallback( windows[i], cursor_position_callback );
		glfwSetMouseButtonCallback( windows[i], mouse_button_callback );
		glfwSetScrollCallback( windows[i], scroll_callback ); 
		
    }
	glfwSetWindowPos( windows[0], 0, 0 );	// Window 1
	glfwSetWindowPos( windows[1], 450, 0 );	// Window 2
	glfwSetWindowPos( windows[2], 900, 0 );	// Window 3
	glfwSetWindowPos( windows[3], 1350, 0 );// Window 4	

    // Make a context current BEFORE loading GLAD
    glfwMakeContextCurrent(windows[1]);
    glfwSwapInterval(0);
    glfwFocusWindow(windows[1]);




    // ---------- GLAD must be loaded here ----------
    
    
    if( !gladLoadGLLoader(( GLADloadproc )glfwGetProcAddress )) 
	{
		fprintf( stderr, "Failed to initialize glad\n" );
		for( int i = 0; i < 3; ++i )
		glfwDestroyWindow( windows[i] );
		glfwTerminate( );
		return EXIT_FAILURE;
	}
    printf("GLAD loaded successfully.\n");
     
        
    g_CTRL = &CTRL;
    g_win2 = windows[1];

    glfwSetKeyCallback( windows[1], key_callback );
    keyboard_init(windows[1], &CTRL);
    printf("Keyboard active only on Window 1.\n");    
    
    
#if 0    
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        fprintf(stderr, "Failed to initialize GLAD\n");
        return EXIT_FAILURE;
    }
    // ----------------------------------------------
#endif

//=========================================================================================================================

	printf( "\n\n=============================================================================================\n" );
	printf( " AeroSim 02\n" );
	printf( " Cessna 182 Flight Simulation\n" );
	printf( " USe ESC on grphics window to exit program \n" );
	printf( " Needed to reset terminal to normal mode after progrm ends \n\n" );
	printf( " OpenGL version : %s\n", glGetString( GL_VERSION ));
	printf( " Renderer : %s\n", glGetString( GL_RENDERER ));
	printf( "=============================================================================================\n" );
	printf( " REALTIME										 \n" );
	int i;
	for( i=0; i<=18; i++ ){ printf( "\n" ); }






	while( !CTRL.terminate )
	{
		Process_Timing( &CTRL );
	}






#if 0
    const double TARGET_FPS = 20.0;
    const double FRAME_TIME = 1.0 / TARGET_FPS;

    while (!glfwWindowShouldClose(windows[2]))
    {
        double frameStart = glfwGetTime();

        glfwPollEvents();

        glfwMakeContextCurrent(windows[2]);
        glClearColor(0.12f, 0.12f, 0.15f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glfwSwapBuffers(windows[2]);

        for (int i = 0; i < 4; i++) {
            if (i == 2) continue;
            glfwMakeContextCurrent(windows[i]);
            glClearColor(0.08f, 0.08f, 0.10f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);
            glfwSwapBuffers(windows[i]);
        }

        double elapsed = glfwGetTime() - frameStart;
        if (elapsed < FRAME_TIME)
            usleep((useconds_t)((FRAME_TIME - elapsed) * 1000000.0));
    }
#endif

    for (int i = 0; i < 4; i++)
        if (windows[i]) glfwDestroyWindow(windows[i]);

    glfwTerminate();
    return EXIT_SUCCESS;
}


#endif





#if 0
//========================================================================================
int main( int argc, char ** argv ) 
{
 struct Cntrls_t	CTRL = {0};
 		AeroModel 	aero = {0};
 
//==============================================================================================

	printf( "========================================================================================\n" );
	printf( " AeroSim 02\n" );
	printf( " Cessna 182 Flight Simulation\n" );
	printf( " USe ESC on grphics window to exit program \n" );
	printf( " Needed to reset terminal to normal mode after progrm ends \n" );
//================================================================================================

	if( !AeroModel_Init( &aero )) return 1;
	AeroModel_PrintSummary( &aero );
 
	double alpha = 8.0;
	// double mach = 0.3;

	double CL = 0.0;
	double mach = 1.1;

	CL = CoeffTable2D_Interpolate( &aero.CL_alpha_mach, alpha, mach );
	printf( "2D: At Alpha=%.2f deg, → CL = %.4f\n", alpha, CL );
	CL = CoeffTable1D_Interpolate( &aero.CL_alpha, alpha );
	printf( "1D: At Alpha=%.2f deg, → CL = %.4f\n", alpha, CL );

	// AeroModel_Destroy( &aero );

//==============================================================================================

//	set_nonblocking_keyboard( );
	UDP_init( );
	Timer_Initialization( );
	glutInit( &argc, argv );
	glfwSetErrorCallback( error_callback );
//==============================================================================================
	
	if( !glfwInit( ) ) 
	{
		fprintf( stderr, "Failed to initialize GLFW\n" );
		return EXIT_FAILURE;
	}
	/* OpenGL 3.0 Compatibility profile( fixed-function still available ), Works with OpenGL 4.6 */
	glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
	glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 0 );   
    // glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);	 // this works for OpenGL 3.0, not OpenGL 3.3
	/* Do NOT set GLFW_OPENGL_CORE_PROFILE */	
	//   GLFWwindow* windows[4] = {NULL};
//================================================================

	const char* titles[4] = 
	{
		"Window 1 - 6DOF",
		"Window 2 - PFD",
		"Window 3 - Course Map",
		"Window 4 - Controls",
	};

	/* Create the four windows */
	for( int i = 0; i <= 3; ++i ) 
	{
		windows[i] = glfwCreateWindow( 450, 450, titles[i], NULL, NULL );
		if( !windows[i] ) 
		{
			fprintf( stderr, "Failed to create GLFW window %d\n", i + 1 );
			for( int j = 0; j < i; ++j ) glfwDestroyWindow( windows[j] );
			glfwTerminate( );
			return EXIT_FAILURE;
		}

		glfwSetFramebufferSizeCallback( windows[i], framebuffer_size_callback ); 
		glfwSetWindowUserPointer( windows[i], &CTRL );
		glfwSetCursorPosCallback( windows[i], cursor_position_callback );
		glfwSetMouseButtonCallback( windows[i], mouse_button_callback );
		glfwSetScrollCallback( windows[i], scroll_callback ); 
		
	}
	glfwSetWindowPos( windows[0], 0, 0 );	// Window 1
	glfwSetWindowPos( windows[1], 450, 0 );	// Window 2
	glfwSetWindowPos( windows[2], 900, 0 );	// Window 3
	glfwSetWindowPos( windows[3], 1350, 0 );// Window 4	

    glfwMakeContextCurrent(windows[1]);
    glfwSwapInterval(0);
    glfwFocusWindow(windows[1]);
    keyboard_init(windows[2], &CTRL);

 //   Cntrls_t CTRL = {0};
    g_CTRL = &CTRL;
    g_win2 = windows[1];

    glfwSetKeyCallback( windows[1], key_callback );
    printf("Keyboard active only on Window 2.\n");

#if 0
	window = glfwCreateWindow( 500, 500, "OpenGL 3.0 Fixed-Function( no shaders )", NULL, NULL );
	if( !window ) 
	{
		fprintf( stderr, "Failed to create GLFW window\n" );
		glfwTerminate( );
		return EXIT_FAILURE;
	}
	glfwSetWindowPos( window, 300, 100 ); 
	glfwMakeContextCurrent( window );
	glfwSetFramebufferSizeCallback( window, framebuffer_size_callback );
#endif

	/* Make the first window current so we can load GLAD once */
//	glfwMakeContextCurrent( windows[0] );	
//	glfwSwapInterval( 0 ); // vsync 1 : monitor synce 0: is no synce. 

	if( !gladLoadGLLoader(( GLADloadproc )glfwGetProcAddress )) 
	{
		fprintf( stderr, "Failed to initialize glad\n" );
		for( int i = 0; i < 3; ++i )
		glfwDestroyWindow( windows[i] );
		glfwTerminate( );
		return EXIT_FAILURE;
	}


//===========================================================================================================================================

	printf( "\n\n=============================================================================================\n" );
	printf( " AeroSim 02\n" );
	printf( " Cessna 182 Flight Simulation\n" );
	printf( " USe ESC on grphics window to exit program \n" );
	printf( " Needed to reset terminal to normal mode after progrm ends \n\n" );
	printf( " OpenGL version : %s\n", glGetString( GL_VERSION ));
	printf( " Renderer : %s\n", glGetString( GL_RENDERER ));
	printf( "=============================================================================================\n" );
	printf( " REALTIME										 \n" );
	int i;
	for( i=0; i<=18; i++ ){ printf( "\n" ); }



	while( !CTRL.terminate )
	{
		Process_Timing( &CTRL );
	}




	for( int i = 0; i < 3; ++i ) 
	{
		if( windows[i] ) glfwDestroyWindow( windows[i] );
	}
	
	glfwTerminate( );
	
	printf( "\033[0m" ); 
	printf( "\033[?25h" ); 
	system( "stty sane" ); 

printf( "============ Exit Program ==============\n" ); 
	
	return EXIT_SUCCESS;
}
#endif
//================================================================================================================
// EOF














