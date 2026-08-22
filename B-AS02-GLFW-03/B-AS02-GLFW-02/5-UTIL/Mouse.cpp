
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
#include <fcntl.h> // for fcntl, O_NONBLOCK
#include <errno.h> // for errno, EAGAIN, EWOULDBLOCK
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

#include "../structures.h"

//=======================================================================================
// Mouse Functions
//=======================================================================================


void cursor_position_callback( GLFWwindow* window, double xpos, double ypos );
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void scroll_callback( GLFWwindow* window, double xoffset, double yoffset );


void cursor_position_callback( GLFWwindow* window, double xpos, double ypos )
{
	Cntrls_t* CTRL = (Cntrls_t*)glfwGetWindowUserPointer(window);

    static int cnt = 0;
    cnt++;

	// printf(">>>> %f  -- %f  \n",  xpos,  ypos );
    // GLFW gives double; convert to int if your CTRL fields are int
    int x = (int)xpos;
    int y = (int)ypos;

    CTRL->MouseX = x - CTRL->MouseStartX;
    CTRL->MouseY = y - CTRL->MouseStartY;

    if (CTRL->Stick_E )
    {
        CTRL->Stick_R = (float)CTRL->MouseX;
        CTRL->Stick_P = (float)CTRL->MouseY;
   //     CTRL->Stick_Y = (float)CTRL->MouseZ;
        
        //printf(">>>> %d      %d       %d       %d \n", CTRL->Stick_R,   CTRL->Stick_P,  CTRL->Stick_Y,  CTRL->Stick_E  );
    }
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	Cntrls_t* CTRL = (Cntrls_t*)glfwGetWindowUserPointer(window);

    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    int x = (int)xpos;
    int y = (int)ypos;

    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        CTRL->MouseStartX = x;
        CTRL->MouseStartY = y;
  //      CTRL->MouseZ = 0;

        CTRL->Button_Down = 1;

        CTRL->MouseX = x - CTRL->MouseStartX;
        CTRL->MouseY = y - CTRL->MouseStartY;

        CTRL->Stick_E = !CTRL->Stick_E;
        // printf(">>>> %d \n",  CTRL->Stick_Enable );
    }

    if (button == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_PRESS )
    {
     //   CTRL->MouseZ = 0;
    //    CTRL->Stick_Y = (float)CTRL->MouseZ;
    }
}

void scroll_callback( 
GLFWwindow* window, double xoffset, double yoffset )
{
	Cntrls_t* CTRL = (Cntrls_t*)glfwGetWindowUserPointer( window );

    // yoffset > 0 → scroll up, < 0 → scroll down
    
    if( yoffset > 0.0 )
    {
        CTRL->MouseZ += 1;
 		//printf(">>34+1>> %d %f \n", CTRL->MouseZ, yoffset );       
        if (CTRL->MouseZ > 100)
            CTRL->MouseZ = 100;
    }
    else if( yoffset < 0.0 )
    {
        CTRL->MouseZ -= 1;
		//printf(">>34-0>> %d %f \n", CTRL->MouseZ, yoffset );   
        if (CTRL->MouseZ < -100)
            CTRL->MouseZ = -100;
    }
    if( CTRL->Stick_E ) CTRL->Stick_Y = (float)CTRL->MouseZ;
    else CTRL->MouseZ = 0.0; 
    		
    
}


