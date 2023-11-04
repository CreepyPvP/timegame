#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stdio.h>

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/vector_float2.hpp>

#include "defines.hpp"
#include "shader.hpp"
#include "geometry_buffer.hpp"

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

    #ifndef DEBUG
    global_window.width = 1920;
    global_window.height = 1080;
    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    #else
    global_window.width = 960;
    global_window.height = 540;
    GLFWmonitor* monitor = NULL;
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

static glm::mat4 setup_projection() {
    // NOTE: The game is running at a 960x540 resolution
    return glm::ortho(
        -480.0f, 
        480.0f, 
        -270.0f, 
        270.0f, 
        .1f,
        1000.0f
    );
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

    glm::mat4 projection = setup_projection();

    unsigned int gpu_buffers = setup_gpu_buffers();
    GeometryBuffer draw_buffer;
    draw_buffer.init();

    GL(glClearColor(0.1, 0.1, 0.1, 1));

    while (!glfwWindowShouldClose(global_window.handle)) {
        if (glfwGetKey(global_window.handle, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(global_window.handle, true);
        }

        GL(glClear(GL_COLOR_BUFFER_BIT));

        draw_buffer.start_batch();
        draw_buffer.draw_rect(100, 100, 100, 100);
        Batch batch = draw_buffer.end_batch();

        draw_buffer.update_gpu_buffers(gpu_buffers);
        draw_buffer.reset();

        GL(glUseProgram(shader.id));
        set_mat4(shader.projection, &projection);
        GL(glBindVertexArray(gpu_buffers));
        GL(glDrawElements(GL_TRIANGLES, 
                          batch.count, 
                          GL_UNSIGNED_INT, 
                          (void*) (batch.index * sizeof(unsigned int))));

        glfwSwapBuffers(global_window.handle);
        glfwPollEvents();
    }

    glfwTerminate();
}
