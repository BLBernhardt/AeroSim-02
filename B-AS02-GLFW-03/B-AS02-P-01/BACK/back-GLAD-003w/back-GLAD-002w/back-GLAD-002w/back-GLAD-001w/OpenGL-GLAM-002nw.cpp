
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#if 0
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <unistd.h>
#include <GL/freeglut.h>
#include <termios.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <signal.h>
#include <time.h>
#include <sys/time.h>
#include <fcntl.h>      // for fcntl, O_NONBLOCK
#include <errno.h>      // for errno, EAGAIN, EWOULDBLOCK



#include "structures.h"
#include "7-MATH/Math.h"
#include "7-MATH/Quaternion.h"
//#include "2-MODEL/aero_model.h"
//#include "2-MODEL/coeff_table.h"
#include "IPC_UDP_Common.h"
//#include "4-IPC/Server_TxRx.h"

struct Math_3D_t   	M_3Dg;
struct Cntrls_t    	CTRLg;
struct Aerodyn_t   	Aero_Dg;
struct Display_t   	DSPLg;
struct Client_Tx_t  TMrxg     = {0}; 
    
//======  Graphics ==============================

 void 	drawAxis( void );
 void 	drawAirvehicle(void);
// void 	Realtime_Calcs( float ); 

 void  	Flight_Stick( int, int ); 
 void 	draw_2D( );
 void   display( void );
 int    YMAXSCREEN, XMAXSCREEN;     // default 500, see inti in main.c 

//====== Numerical ================================

 Quaternion4	vehicle_orientation_q;  
 
 void Process_Timing( );  
// void Aero_Computations( float, const AeroModel* Aero_M, TM_Param_t *TMtx  ); 
 void TM_Print( KB_State_t *kb, TM_Param_t *TMrx, Client_Tx_t  *TMtx );
 void Realtime_Calcs( float dt, Aerodyn_t *Aero_Dg );
//  AeroModel* Aero_M = NULL;
 #endif
//=====================================================================================================




static void error_callback(int error, const char* description)
{
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

static void draw_triangle(float r, float g, float b)
{
    glClearColor(0.08f, 0.10f, 0.14f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glBegin(GL_TRIANGLES);
        glColor3f(r, g, b);
        glVertex2f( 0.0f,  0.5f);
        glVertex2f(-0.5f, -0.5f);
        glVertex2f( 0.5f, -0.5f);
    glEnd();
}

int main(void)
{
    glfwSetErrorCallback(error_callback);

    if (!glfwInit()) 
    {
        fprintf(stderr, "Failed to initialize GLFW\n");
        return EXIT_FAILURE;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    /* Create three windows */
    GLFWwindow* window1 = glfwCreateWindow(400, 300, "Window 1 - Red",   NULL, NULL);
    GLFWwindow* window2 = glfwCreateWindow(400, 300, "Window 2 - Green", NULL, NULL);
    GLFWwindow* window3 = glfwCreateWindow(400, 300, "Window 3 - Blue",  NULL, NULL);

    if (!window1 || !window2 || !window3) {
        fprintf(stderr, "Failed to create one or more GLFW windows\n");
        glfwTerminate();
        return EXIT_FAILURE;
    }

    /* Position the windows so they don't overlap */
    glfwSetWindowPos(window1, 100, 100);
    glfwSetWindowPos(window2, 520, 100);
    glfwSetWindowPos(window3, 940, 100);

    /* Make the first context current and load glad once */
    glfwMakeContextCurrent(window1);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        fprintf(stderr, "Failed to initialize glad\n");
        glfwTerminate();
        return EXIT_FAILURE;
    }

    glfwSetFramebufferSizeCallback(window1, framebuffer_size_callback);
    glfwSetFramebufferSizeCallback(window2, framebuffer_size_callback);
    glfwSetFramebufferSizeCallback(window3, framebuffer_size_callback);

    glfwSwapInterval(1);

    printf("OpenGL version : %s\n", glGetString(GL_VERSION));
    printf("Renderer       : %s\n", glGetString(GL_RENDERER));

    /* Main loop - keep running while any window is open */
    while (!glfwWindowShouldClose(window1) ||
           !glfwWindowShouldClose(window2) ||
           !glfwWindowShouldClose(window3))
    {
        /* ----- Window 1 ----- */
        if (!glfwWindowShouldClose(window1)) {
            glfwMakeContextCurrent(window1);
            draw_triangle(1.0f, 0.2f, 0.2f);   /* red */
            glfwSwapBuffers(window1);
        }

        /* ----- Window 2 ----- */
        if (!glfwWindowShouldClose(window2)) {
            glfwMakeContextCurrent(window2);
            draw_triangle(0.2f, 1.0f, 0.2f);   /* green */
            glfwSwapBuffers(window2);
        }

        /* ----- Window 3 ----- */
        if (!glfwWindowShouldClose(window3)) {
            glfwMakeContextCurrent(window3);
            draw_triangle(0.2f, 0.2f, 1.0f);   /* blue */
            glfwSwapBuffers(window3);
        }

        glfwPollEvents();
    }

    glfwDestroyWindow(window1);
    glfwDestroyWindow(window2);
    glfwDestroyWindow(window3);
    glfwTerminate();
    return EXIT_SUCCESS;
}
