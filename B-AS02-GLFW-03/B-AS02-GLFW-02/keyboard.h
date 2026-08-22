#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <GLFW/glfw3.h>
#include "structures.h"

//typedef struct {
//    int key;
//} Cntrls_t;

void keyboard_init(GLFWwindow* window, Cntrls_t* ctrl);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

#endif

//=======================================================================================

