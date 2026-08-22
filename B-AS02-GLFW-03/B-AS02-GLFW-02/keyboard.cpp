
//=======================================================================================
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
#include <stdio.h>
#include "structures.h"


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

		// Course Alpha Adjust
		case '1':  
       		CTRL->angle_alpha -= 0.01;
       	 	if( CTRL->angle_alpha < -20.0 ) CTRL->angle_alpha = -20.0;
	  	break;
       	case '2':  
       		CTRL->angle_alpha += 0.01;
       	 	if( CTRL->angle_alpha > + 20.0 ) CTRL->angle_alpha = +20.0;
	  	break;
	  	
	  	// Fine Alpha Adjust
		case '3':  
       		CTRL->angle_alpha -= 0.001;
       	 	if( CTRL->angle_alpha < -20.0 ) CTRL->angle_alpha = -20.0;
	  	break;
       	case '4':  
       		CTRL->angle_alpha += 0.001;
       	 	if( CTRL->angle_alpha > + 20.0 ) CTRL->angle_alpha = +20.0;
	  	break;
	  	
	  	// Elevator command   Up, Zero, Down 
       	case '0':
       		CTRL->Elevator_Cmd += 0.02;
       	 	if( CTRL->Elevator_Cmd > +1.0 ) CTRL->Elevator_Cmd = +1.0;
	  	break;
       	case '9':
       		CTRL->Elevator_Cmd = 0.0;
	  	break;	  	
       	case '8':
       		CTRL->Elevator_Cmd -= 0.02;
       	 	if( CTRL->Elevator_Cmd < -1.0 ) CTRL->Elevator_Cmd = -1.0;
	  	break;
	  	
	    // Throttle command   Up, Zero, Down 
	  	case 'e':
       		CTRL->Throttle_Cmd  += 0.02;
       	 	if( CTRL->Throttle_Cmd  > +1.0 ) CTRL->Throttle_Cmd = +1.0;
	  	break;
       	case 'd':
       		CTRL->Throttle_Cmd  = 0.0;
	  	break;
       	case 'c':
       		CTRL->Throttle_Cmd  -= 0.02;
       	 	if( CTRL->Throttle_Cmd   < 0 ) CTRL->Throttle_Cmd = 0;
	  	break;
	  	
	    // Pitch command   Up, Zero, Down 
	  	case 'P': case 'p':
            CTRL->Pitch_Cmd += 0.01;
            if( CTRL->Pitch_Cmd > 1.0 ) CTRL->Pitch_Cmd = 1.0;	  	
	  	break;	  	
	  	case 'O': case 'o': 
            CTRL->Pitch_Cmd = 0.0;;
      	break;
	  	case 'I': case 'i':       
            CTRL->Pitch_Cmd -= 0.01;
            if( CTRL->Pitch_Cmd < -1.0 ) CTRL->Pitch_Cmd = -1.0;
      	break;
      	
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
