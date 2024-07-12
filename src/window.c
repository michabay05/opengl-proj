#include "glad/glad.h"
#include "window.h"

#include <stdio.h>
#include <string.h>

struct Window window;

void a_debug_callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
                      const GLchar *message, const void *userParam);
void frame_resize_callback(GLFWwindow *window, int width, int height);
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);

bool window_create(uint32_t width, uint32_t height, const char *title)
{
    if (glfwInit() != GLFW_TRUE) {
        printf("[ERROR] Unable to initialize glfw\n");
        return false;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_CONTEXT_DEBUG, GLFW_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

    GLFWwindow *win_ptr = glfwCreateWindow(width, height, title, NULL, NULL);

    if (win_ptr == NULL) {
        printf("[ERROR] Unable to create a GLFW window\n");
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(win_ptr);
    window.handle = win_ptr;
    window.width = width;
    window.height = height;

    glfwSetFramebufferSizeCallback(window.handle, frame_resize_callback);
    glfwSetKeyCallback(window.handle, key_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        printf("[ERROR] Unable to intialize GLAD.\n");
        glfwTerminate();
        return false;
    }

    return true;
}

void window_clear_input(void) {
    memset(window.input.keys, 0, KEYS_COUNT);
}

void key_callback(GLFWwindow *win, int key, int scancode, int action, int mods)
{
    (void)win;
    (void)scancode;
    (void)mods;

    if (key < 0)
        return;
    switch (action) {
        case GLFW_PRESS:
            window.input.keys[key] = BS_PRESSED;
            break;
        case GLFW_RELEASE:
            window.input.keys[key] = BS_RELEASED;
            break;
    }
}

void frame_resize_callback(GLFWwindow *win, int width, int height)
{
    (void)win;
    window.width = width;
    window.height = height;
}
