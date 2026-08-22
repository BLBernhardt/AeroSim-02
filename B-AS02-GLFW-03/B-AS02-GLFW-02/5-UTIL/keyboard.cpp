
//=======================================================================================
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <unistd.h>
#include <termios.h>
#include <errno.h>
#include <sys/ioctl.h>
//#include <signal.h>
//#include <time.h>
//#include <sys/time.h>
//#include <fcntl.h> 
//#include <errno.h> 
//#include <stdio.h>
//#include <stdlib.h>
//#include <sys/select.h>

#include "keyboard.h"

#include "../structures.h"


static Cntrls_t*   s_CTRL = NULL;
static GLFWwindow* s_win  = NULL;

void keyboard_init(GLFWwindow* window, Cntrls_t* ctrl)
{
    s_win  = window;
    s_CTRL = ctrl;
    glfwSetKeyCallback(window, key_callback);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (window != s_win) return;
    if (action != GLFW_PRESS) return;

    if (s_CTRL) {
        s_CTRL->key = key;

        if (key >= 32 && key <= 126)
            printf("key = '%c'\n", (char)key);
        else
            printf("key = %d (special)\n", key);
    }

    if (key == GLFW_KEY_ESCAPE)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}
//=======================================================================================


void keyboard( unsigned char key,  Cntrls_t* CTRL  )
{

    //printf("key -  %d  \n", key );  //Test Code
   CTRL->key = key;
 
    switch(key) 
    {
		// Exit program
	   	case 27:   //  ascii ESC  235 dec
	  		CTRL->abort = 1;	
 			CTRL->terminate = 1;
            printf( "\033[0m" );   
            printf( "\033[?25h" ); 
            system( "stty sane" ); 
            printf( "Exit Program\n" );    
        break;


	    // Pitch Trim  Up, Zero, Down 
	  	case '1': 
            CTRL->Elv_Trim+= 0.01;
            if( CTRL->Elv_Trim> 1.0 ) CTRL->Elv_Trim= 1.0;	  	
	  	break;	  	
	  	case '2':  
            CTRL->Elv_Trim= 0.0;;
      	break;
	  	case '3':       
            CTRL->Elv_Trim-= 0.01;
            if( CTRL->Elv_Trim< -1.0 ) CTRL->Elv_Trim= -1.0;
      	break;


		// Roll Trim  CCW, Cent, CW
	  	case 'Q': case 'q': 
            CTRL->Ail_Trim+= 0.01;
            if( CTRL->Ail_Trim> 1.0 ) 	CTRL->Ail_Trim= 1.0;	  	
	  	break;	  	
	  	case 'W': case 'w': 			CTRL->Ail_Trim= 0.0;;
      	break;
	  	case 'E': case 'e':      
            CTRL->Ail_Trim-= 0.01;
            if( CTRL->Ail_Trim< -1.0 ) 	CTRL->Ail_Trim= -1.0;
      	break;
       	 	
       	 	
	  	// Yaw Trim  L, cent, R
	  	case 'A': case 'a': 
            CTRL->Rud_Trim += 0.01;
            if( CTRL->Rud_Trim> 1.0 ) 	CTRL->Rud_Trim= 1.0;	  	
	  	break;	  	
	  	case 'S': case 's': 			CTRL->Rud_Trim = 0.0;
      	break;
	  	case 'D': case 'd':      
            CTRL->Rud_Trim -= 0.01;
            if( CTRL->Rud_Trim< -1.0 ) 	CTRL->Rud_Trim= -1.0;
      	break;
      	
      	
 	  	// Yaw Trim  L, cent, R
	  	case 'Z': case 'z': 
            CTRL->Throttle_Cmd += 0.01;
            if( CTRL->Ail_Trim> 1.0 ) 	CTRL->Throttle_Cmd = 1.0;	  	
	  	break;	  	
	  	case 'X': case 'x': 			CTRL->Throttle_Cmd = 0.0;;
      	break;
	  	case 'C': case 'c':      
            CTRL->Throttle_Cmd -= 0.01;
            if( CTRL->Ail_Trim< -1.0 ) 	CTRL->Throttle_Cmd = -1.0;
      	break;     	


	    // Gear    1: Dwon   0: Up
	  	case 'G': case 'g':
	  		CTRL->Gear_Cmd = !CTRL->Gear_Cmd;


        // Pitch impulse command   Up, Down 
      	case 'T': case 't': 
            CTRL->Pitch_impulse = +1;
            printf(" PI + \n");
      	break;
	  	case 'Y': case 'y':       
            CTRL->Pitch_impulse =  -1;
            printf(" PI - \n");
      	break;


		default:
	    break;
	    
    }


 }
