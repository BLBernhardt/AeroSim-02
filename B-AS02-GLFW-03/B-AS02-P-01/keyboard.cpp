
//=======================================================================================

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

