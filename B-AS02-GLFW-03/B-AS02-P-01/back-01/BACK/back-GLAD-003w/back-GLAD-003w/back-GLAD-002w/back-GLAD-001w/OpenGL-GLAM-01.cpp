#include <stdio.h>
#include <stdlib.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

static void error_callback(int error, const char* description)
{
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

int main(void)
{
    glfwSetErrorCallback(error_callback);

    if (!glfwInit()) {
        fprintf(stderr, "Failed to initialize GLFW\n");
        return EXIT_FAILURE;
    }

    /* OpenGL 3.0 Compatibility profile (fixed-function still available) */
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    /* Do NOT set GLFW_OPENGL_CORE_PROFILE */

    GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL 3.0 Fixed-Function (no shaders)", NULL, NULL);
    if (!window) {
        fprintf(stderr, "Failed to create GLFW window\n");
        glfwTerminate();
        return EXIT_FAILURE;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSwapInterval(1);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        fprintf(stderr, "Failed to initialize glad\n");
        glfwDestroyWindow(window);
        glfwTerminate();
        return EXIT_FAILURE;
    }

    printf("OpenGL version : %s\n", glGetString(GL_VERSION));
    printf("Renderer       : %s\n", glGetString(GL_RENDERER));

    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.08f, 0.10f, 0.14f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        /* Draw a simple triangle with fixed-function pipeline */
        glBegin(GL_TRIANGLES);
            glColor3f(1.0f, 0.2f, 0.2f);   /* red */
            glVertex2f( 0.0f,  0.5f);

            glColor3f(0.2f, 1.0f, 0.2f);   /* green */
            glVertex2f(-0.5f, -0.5f);

            glColor3f(0.2f, 0.2f, 1.0f);   /* blue */
            glVertex2f( 0.5f, -0.5f);
        glEnd();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return EXIT_SUCCESS;
}