#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stdio.h>

#include "defines.hpp"
#include "shader.hpp"

struct Window {
    int width;
    int height;
    GLFWwindow* handle;
};

static Window global_window;

static void resize_cb(GLFWwindow *window, int width, int height)
{
    global_window.width = width;
    global_window.height = height;
}

static int init_window() 
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 4);

    global_window.width = 1280;
    global_window.height = 720;
    GLFWmonitor* monitor = NULL;
    #ifndef DEBUG
    monitor = glfwGetPrimaryMonitor();
    #endif
    global_window.handle = glfwCreateWindow(global_window.width,
                                            global_window.height,
                                            "Timegame",
                                            monitor,
                                            NULL);
    if (!global_window.handle) {
        printf("Failed to create window\n");
        return 1;
    }

    glfwSetFramebufferSizeCallback(global_window.handle, 
                                   resize_cb);
    glfwMakeContextCurrent(global_window.handle);
    return 0;
}

int main() 
{
    if (init_window()) {
        printf("Failed to open window\n");
        return 1;
    }
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        printf("failed to load required extensions\n");
        return 3;
    }

    DefaultShader shader;
    if (load_default("../shader/v_default.glsl", 
                     "../shader/f_default.glsl", 
                     &shader)) {
        return 2;
    }

    while (!glfwWindowShouldClose(global_window.handle)) {
        if (glfwGetKey(global_window.handle, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(global_window.handle, true);
        }

        glfwSwapBuffers(global_window.handle);
        glfwPollEvents();
    }

    glfwTerminate();
}
