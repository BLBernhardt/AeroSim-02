


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>
#include <errno.h>

#include "structures.h"



static struct termios orig_termios;
static int orig_flags;

void restore_terminal(void) {
    tcsetattr(STDIN_FILENO, TCSANOW, &orig_termios);
    fcntl(STDIN_FILENO, F_SETFL, orig_flags);
}

void set_nonblocking_keyboard(void) {
    if (tcgetattr(STDIN_FILENO, &orig_termios) < 0) {
        perror("tcgetattr");
        exit(1);
    }
    orig_flags = fcntl(STDIN_FILENO, F_GETFL, 0);

    atexit(restore_terminal);

    struct termios raw = orig_termios;
    raw.c_lflag &= ~(ICANON | ECHO | ISIG);
    raw.c_cc[VMIN]  = 0;
    raw.c_cc[VTIME] = 0;
    tcsetattr(STDIN_FILENO, TCSANOW, &raw);

    fcntl(STDIN_FILENO, F_SETFL, orig_flags | O_NONBLOCK);
}

int getkey(void) {
    unsigned char c;
    ssize_t n = read(STDIN_FILENO, &c, 1);

    if (n == 1)
        return (int)c;

    if (n < 0 && (errno == EAGAIN || errno == EWOULDBLOCK))
        return -1;

    return -1;
}



int keyboard(  Cntrls_t* CTRL )
{
	unsigned char key;
    //printf("key -  %d  \n", key );  //Test Code

   key = getkey( );
 
    switch(key) 
    {
		// Exit program
	   	case 27:   //  ascii ESC  235 dec
	        CTRL->terminate = 1;
	  		CTRL->abort = 1;	
 
  
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

	return key;
 }




