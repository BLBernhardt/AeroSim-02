
//================================================================================================================================================
// OpenGL_GLAM.c

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
#include <fcntl.h> // for fcntl, O_NONBLOCK
#include <errno.h> // for errno, EAGAIN, EWOULDBLOCK
#include <stdio.h>
#include <stdlib.h>

//----------------------------------------------------------------------------------------
// oder of these 4 items must be GL/freeglut.h after #include <glad/glad.h> #include <GLFW/glfw3.h>
#include <glad/glad.h> // must be first OpenGL-related header
#include <GLFW/glfw3.h>
#define FREEGLUT_LIB_PRAGMAS 0 // Tell FreeGLUT not to include the system OpenGL headers
#include <GL/freeglut.h>
//----------------------------------------------------------------------------------------

#include "structures.h"
#include "7-MATH/Math.h"
#include "7-MATH/Quaternion.h"
//#include "2-MODEL/aero_model.h"
//#include "2-MODEL/coeff_table.h"
#include "IPC_UDP_Common.h"
#include "4-IPC/Client_TxRx.h"


//struct Math_3D_t 	M_3Dg;
//struct Cntrls_t 	CTRLg;
struct Aerodyn_t 	Aero_Dg;
struct Display_t 	DSPLg;
struct Client_Tx_t  TMtxg = {0}; 
struct Server_Tx_t  TMrxg = {0};  

//====== Graphics ==============================

 void 	drawAxis( void );
 void 	drawAirvehicle( void );
 void 	Flight_Stick( int, int ); 
 void 	draw_2D();
 void display( void );
 void Render_2D_Win1( Server_Tx_t* ); 
 int YMAXSCREEN, XMAXSCREEN; // default 500, see inti in main.c 

static void error_callback( int error, const char* description ){ fprintf( stderr, "GLFW Error %d: %s\n", error, description ); }
static void framebuffer_size_callback( GLFWwindow* window, int width, int height ){ glViewport( 0, 0, width, height ); }
void Render_2D_Win2( Cntrls_t* CTRL );

//GLFWwindow* window = NULL;   // global

void Render_Window1(GLFWwindow* windows[3]);
void Render_Window2(GLFWwindow* windows[3]);
void Render_Window3(GLFWwindow* windows[3]);

    GLFWwindow* windows[3] = {NULL, NULL, NULL};

//====== Numerical ================================

 Quaternion4	vehicle_orientation_q; 
 
	 void Process_Timing(); 
	// void Aero_Computations( float, const AeroModel* Aero_M, Server_Tx_t *TMtx ); 
	 void TM_Print( KB_State_t *kb, Server_Tx_t *TMrx, Client_Tx_t *TMtx );
	 void Realtime_Calcs( float dt, Aerodyn_t *Aero_Dg );
	// AeroModel* Aero_M = NULL;
	 void stroke_output( GLfloat x, GLfloat y, GLfloat size, const char *text );
	 void Render_2D_Win1();

//====== IPC UDP =================================
 
// 	void UDP_Tx( Client_Tx_t* TMtx );
// 	void UDP_Rx( Server_Tx_t* TMrx );
 
	int tx_sockfd, rx_sockfd;
	struct sockaddr_in tx_addr, rx_addr;

	int tx_sock;
	struct sockaddr_in dest_addr;

	volatile sig_atomic_t running = 1;

	void handle_sigint(int sig) 
	{
	    (void)sig;
	    running = 0;
	}

//=======================================================================================
//      Time and Interupt Functions
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

static struct timespec prev_mono = {0};
static int first_call = 1;
static double sim_time_sec = 0.0;

float Time_Update( Server_Tx_t *TMtx )
{
    struct timespec ts;

    if (clock_gettime(CLOCK_MONOTONIC_RAW, &ts) == -1) 
    {
        perror(">>>> error: clock_gettime(CLOCK_MONOTONIC_RAW)");
        TMtx->dt = 0.0f;
        return 0.0f;
    }

    if (first_call) 
    {
        time_t real_sec = time(NULL);
        struct tm *tm_info = localtime(&real_sec);
        
        if (tm_info) 
        {
            TMtx->hours = tm_info->tm_hour;
            TMtx->min   = tm_info->tm_min;
            TMtx->sec   = tm_info->tm_sec;
            TMtx->mSec  = (uint)(ts.tv_nsec / 1000000U);
        } else 
        {
            TMtx->hours = 0;
            TMtx->min   = 0;
            TMtx->sec   = 0;
            TMtx->mSec  = 0;
        }

        sim_time_sec = (double)real_sec + (double)TMtx->mSec / 1000.0;
        first_call = 0;
        prev_mono = ts;
        TMtx->dt = 0.0f;
    } else 
    {
        // High precision monotonic delta
        long long sec_diff  = ts.tv_sec - prev_mono.tv_sec;
        long      nsec_diff = ts.tv_nsec - prev_mono.tv_nsec;
        TMtx->dt = (float)sec_diff + (float)nsec_diff * 1e-9f;

        sim_time_sec += TMtx->dt;

        time_t sim_sec = (time_t)sim_time_sec;
        struct tm *tm_info = localtime(&sim_sec);
        
        if (tm_info) 
        {
            TMtx->hours = tm_info->tm_hour;
            TMtx->min   = tm_info->tm_min;
            TMtx->sec   = tm_info->tm_sec;
            TMtx->mSec  = (uint)((sim_time_sec - (double)sim_sec) * 1000.0);
        } else 
        {
            TMtx->hours = 0;
            TMtx->min   = 0;
            TMtx->sec   = 0;
            TMtx->mSec  = 0;
        }
    }

    prev_mono = ts;
    return TMtx->dt;
}

//=======================================================================================


//=======================================================================================
//      Frame Executive 
//=======================================================================================
// Returns true if at least one window is still open, false if all are closed
bool Update_Graphics(GLFWwindow* windows[3])
{
    // Check if any window was closed
    if ( (windows[0] && glfwWindowShouldClose(windows[0])) ||
         (windows[1] && glfwWindowShouldClose(windows[1])) ||
         (windows[2] && glfwWindowShouldClose(windows[2])) )
    {
        // Force all windows to close
        if (windows[0]) glfwSetWindowShouldClose(windows[0], GLFW_TRUE);
        if (windows[1]) glfwSetWindowShouldClose(windows[1], GLFW_TRUE);
        if (windows[2]) glfwSetWindowShouldClose(windows[2], GLFW_TRUE);
        return false;
    }

    // Render each window
    Render_Window1(windows);
    Render_Window2(windows);
    Render_Window3(windows);

    glfwPollEvents();
    return true;
}



void Process_Timing(  )
{
    static int count_1000  	= 0;
    static int count_100 	= 0;    
    static int count_20 	= 0;
    static int count_10 	= 0;    
      
 	static KB_State_t kb     = {0};  
 	
 	struct Cntrls_t 	CTRL;
// 	static Aerodyn_t  Aero_Dg = {0};
    
    //  glut_idle_process ( Process_Timming )  executes free wheel at 1 meg per second or more.  
    if( interrupt_flag == 1 )  // Timer Interupt set for 1000 interupts per second. See Timer Initialization
    {
    	interrupt_flag = 0;
    	count_1000++;	
     	count_100++;      	
     	count_20++; 
     	count_10++;      	  	
    	
    	if( count_1000  > 1000 )  //  1.0 FPS 
    	{   count_1000 = 0;

		//	printf(">>>>fame count %d \n", TMtx.frame_cnt);	
		}
    	if( count_10 > 10 )  //  100 FPS 
    	{   count_10 = 0;   	
			CTRL.Elevator_Cmd = TMtxg.pitch_cmd;
			CTRL.Aileron_Cmd  = TMtxg.roll_cmd;
			CTRL.Rudder_Cmd   = TMtxg.yaw_cmd;
			CTRL.Throttle_Cmd = TMtxg.thrtl_cmd;
		}		
		
	
		
    	if( count_20 > 20 )  //  50.0 FPS 
    	{   count_20 = 0;
    	    TMrxg.frame_cnt++;

  			IPC_UDP_C_Rx( rx_sockfd, &TMrxg ); 

 		 	glClearColor( 0.08f, 0.10f, 0.14f, 1.0f );
 			glClear( GL_COLOR_BUFFER_BIT );

  			IPC_UDP_C_Rx( rx_sockfd, &TMrxg ); 

 			// Granphics routing for window 1 6dDOF
//			Render_2D_Win1( &TMrxg );
			
			
			
// 			glfwSwapBuffers( window );
// 			glfwPollEvents();   	
    	
//    		glutSetWindow( DSPLg.Window1 );
//    		glutPostRedisplay(  );
    		
//     		glutSetWindow( DSPLg.Window2 );
//    		glutPostRedisplay(  );   		
    		
//    		glutSetWindow( DSPLg.Window3 );
//    		glutPostRedisplay(  );  
    		
//    	    UDP_Tx( &TMtxg );
    		//printf(" Frame 50 FPS \n" );
		}	
		
		
    	if( count_100 > 100 )  //  10.0 FPS 
    	{   count_100 = 0;
//			float dt = 
			Time_Update( &TMrxg );
//			TMtxg.dt    = dt;
//			TMtxg.key = CTRLg.key;
			IPC_UDP_C_Tx(tx_sock, &dest_addr, &TMtxg ); 			
			TM_Print( &kb, &TMrxg, &TMtxg );
		
		}	
	}
}

//=======================================================================================
// Write alphanumeric characters to graphics - GLUT function will replace later
//=======================================================================================
void stroke_output( GLfloat x, GLfloat y, GLfloat size, const char *text )
{
 const char *p;
 glPushMatrix();
 glTranslatef( x, y, 0 );
 glScalef( size, size, size );
 for( p = text; *p; p++ )
 glutStrokeCharacter( GLUT_STROKE_ROMAN, *p );
 glPopMatrix();
}









void UDP_init()
{
    signal(SIGINT, handle_sigint);
    IPC_UDP_C_Tx_init( &tx_sock, &dest_addr, CLIENT_TX_PORT, DEFAULT_SERVER_IP ); // Client Tx to Server Rx 
    printf("Started Client Tx to Server Rx        port:%d\n",  CLIENT_TX_PORT );
  
    IPC_UDP_C_Rx_init(&rx_sockfd, &rx_addr, SERVER_TX_PORT, DEFAULT_CLIENT_IP );   // Client Rx from Server Tx 
    printf("Started Server Tx to Client Rx        port:%d\n",  SERVER_TX_PORT );
}


void Render_Window1(GLFWwindow* windows[3])
{
    if (!windows[0] || glfwWindowShouldClose(windows[0]))
        return;

    glfwMakeContextCurrent(windows[0]);
    Render_2D_Win1( &TMrxg );
    glfwSwapBuffers(windows[0]);
}

void Render_Window2(GLFWwindow* windows[3])
{
    if (!windows[1] || glfwWindowShouldClose(windows[1]))
        return;

    glfwMakeContextCurrent(windows[1]);

    glClearColor(0.08f, 0.10f, 0.14f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    /* --- Window 2 drawing code goes here --- */
    glBegin(GL_TRIANGLES);
        glColor3f(0.2f, 1.0f, 0.2f);   /* green */
        glVertex2f( 0.0f,  0.5f);
        glVertex2f(-0.5f, -0.5f);
        glVertex2f( 0.5f, -0.5f);
    glEnd();

    glfwSwapBuffers(windows[1]);
}

void Render_Window3(GLFWwindow* windows[3])
{
    if (!windows[2] || glfwWindowShouldClose(windows[2]))
        return;

    glfwMakeContextCurrent(windows[2]);

    glClearColor(0.08f, 0.10f, 0.14f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    /* --- Window 3 drawing code goes here --- */
    glBegin(GL_TRIANGLES);
        glColor3f(0.2f, 0.2f, 1.0f);   /* blue */
        glVertex2f( 0.0f,  0.5f);
        glVertex2f(-0.5f, -0.5f);
        glVertex2f( 0.5f, -0.5f);
    glEnd();

    glfwSwapBuffers(windows[2]);
}


int main( void )
{

	UDP_init();
    Timer_Initialization();
//===================================================================
// UDP Init
   

//===================================================================

 	glfwSetErrorCallback( error_callback );

 	if( !glfwInit() ) 
 	{
 		fprintf( stderr, "Failed to initialize GLFW\n" );
 		return EXIT_FAILURE;
 	}
//================================================================




//================================================================
 	/* OpenGL 3.0 Compatibility profile( fixed-function still available ), Works with OpenGL 4.6 */
 	glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
 	glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 0 );
 	/* Do NOT set GLFW_OPENGL_CORE_PROFILE */

//    GLFWwindow* windows[3] = {NULL, NULL, NULL};
    const char* titles[3] = {
        "Window 1 - Red triangle",
        "Window 2 - Green triangle",
        "Window 3 - Blue triangle"
    };

    /* Create the three windows */
    for (int i = 0; i <= 2; ++i) 
    {
        windows[i] = glfwCreateWindow(500, 500, titles[i], NULL, NULL);
        if (!windows[i]) {
            fprintf(stderr, "Failed to create GLFW window %d\n", i + 1);
            for (int j = 0; j < i; ++j)
                glfwDestroyWindow(windows[j]);
            glfwTerminate();
            return EXIT_FAILURE;
        }
        glfwSetFramebufferSizeCallback(windows[i], framebuffer_size_callback);
    }

	glfwSetWindowPos(windows[0], 100, 100);   // Window 1
	glfwSetWindowPos(windows[1], 650, 100);   // Window 2
	glfwSetWindowPos(windows[2], 1200, 100);  // Window 3



#if 0
 	window = glfwCreateWindow( 500, 500, "OpenGL 3.0 Fixed-Function( no shaders )", NULL, NULL );
 	if( !window ) 
 	{
 		fprintf( stderr, "Failed to create GLFW window\n" );
 		glfwTerminate();
 		return EXIT_FAILURE;
 	}
	glfwSetWindowPos(window, 300, 100); 
 	glfwMakeContextCurrent( window );
 	glfwSetFramebufferSizeCallback( window, framebuffer_size_callback );
#endif
 	
 	/* Make the first window current so we can load GLAD once */
    glfwMakeContextCurrent(windows[0]); 	
 	glfwSwapInterval(0);   // vsync  1 : monitor synce   0: is no synce. 

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        fprintf(stderr, "Failed to initialize glad\n");
        for (int i = 0; i < 3; ++i)
            glfwDestroyWindow(windows[i]);
        glfwTerminate();
        return EXIT_FAILURE;
    }





//==============================================================================================

printf("\n\n=============================================================================================\n");
printf("                                  AeroSim 02\n");
printf("     Cessna 182  Flight Simulation\n");
printf("         USe ESC on grphics window to exit program \n");
printf("         Needed to reset terminal to normal mode after progrm ends \n\n");
 	printf( "OpenGL version : %s\n", glGetString( GL_VERSION ));
 	printf( "Renderer : %s\n", glGetString( GL_RENDERER ));

	printf("=============================================================================================\n");
	printf("                                        REALTIME									    	 \n");
	int i;
	for( i=0; i<=18; i++ ){ printf("\n"); }


    /* Main loop – keep running while at least one window is still open */
    
    
    while (true)
    {
        // Call the graphics update (renders all 3 windows + polls events)
        if (!Update_Graphics( windows ))
            break;   // all windows closed → exit

        // Sleep to target ~50 Hz (20 ms per frame)
        usleep(20000);   // 20 000 microseconds = 20 ms
    }
    
    
    
    
    
    
    #if 0
while (true)
{
    bool any_open = false;

    for (int i = 0; i < 3; ++i) {
        if (windows[i] && !glfwWindowShouldClose(windows[i])) {
            any_open = true;
            break;
        }
    }
    if (!any_open)
        break;

    Render_Window(windows, 1);
    Render_Window(windows, 2);
    Render_Window(windows, 3);

    glfwPollEvents();
}
	#endif
#if 0	
    while (true) 
    {
        bool any_open = false;
        for (int i = 0; i < 3; ++i) 
        {
        	i = 1;
            if (!windows[i] || glfwWindowShouldClose(windows[i]))
                continue;
            any_open = true;
            glfwMakeContextCurrent(windows[i]);
            glClearColor(0.08f, 0.10f, 0.14f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);
            /* Draw a triangle (different color per window just for demonstration) */
            glBegin(GL_TRIANGLES);
            if (i == 0) 	glColor3f(1.0f, 0.2f, 0.2f);   /* red */
            else if (i == 1)glColor3f(0.2f, 1.0f, 0.2f);   /* green */
            else 			glColor3f(0.2f, 0.2f, 1.0f);   /* blue */
            	
            	glVertex2f( 0.0f,  0.5f);
            	glVertex2f(-0.5f, -0.5f);
            	glVertex2f( 0.5f, -0.5f);
            	glEnd();
            glfwSwapBuffers(windows[i];              
        }
    }
#endif
      
        
#if 0
 	while( !glfwWindowShouldClose( window ))
 	{
 	    Process_Timing( );
 	    #if 0
 		glClearColor( 0.08f, 0.10f, 0.14f, 1.0f );
 		glClear( GL_COLOR_BUFFER_BIT );

 		// Granphics routing for window 1 6dDOF
		Render_2D_Win1();
 		glfwSwapBuffers( window );
 		glfwPollEvents();
 		#endif
 	}



 	glfwDestroyWindow( window );
 	glfwTerminate();
 	return EXIT_SUCCESS;
#endif



 	    /* Cleanup */
    for (int i = 0; i < 3; ++i) 
    {
    Process_Timing(  );
        if (windows[i])
            glfwDestroyWindow(windows[i]);
    }
    glfwTerminate();
    return EXIT_SUCCESS;
}

//================================================================================================================
// EOF
