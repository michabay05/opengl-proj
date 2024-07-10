#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

void debug_callback(GLenum source, GLenum type, GLuint id, GLenum severity,
                    GLsizei length, const GLchar *message,
                    const void *userParam);
GLint init_shaders(const char *vertex_path, const char *frag_path);

int main(void) {
    if (glfwInit() != GLFW_TRUE) {
        printf("[ERROR] Unable to initialize glfw\n");
        return 1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_CONTEXT_DEBUG, GLFW_TRUE);

    GLFWwindow *window =
        glfwCreateWindow(800, 600, "OpenGL Project", NULL, NULL);
    if (window == NULL) {
        printf("[ERROR] Unable to create a GLFW window\n");
        glfwTerminate();
        return 1;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        printf("[ERROR] Unable to intialize GLAD.\n");
        glfwTerminate();
        return 1;
    }

    if (GLAD_GL_ARB_debug_output) {
        glEnable(GL_DEBUG_OUTPUT);
        glDebugMessageCallback(debug_callback, NULL);
    }

    glViewport(0, 0, 800, 600);
    // TODO: implement frame resize callback

    GLint shader =
        init_shaders("res/shaders/circle.vert", "res/shaders/circle.frag");

    while (!glfwWindowShouldClose(window)) {
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

GLint init_shaders(const char *vertex_path, const char *fragment_path) {
    GLchar *buf = NULL;
    FILE *f = NULL;
    size_t file_size;

    GLint vertex_shader, fragment_shader;
    bool vertex_success, fragment_success;

    {
        f = fopen(vertex_path, "r");
        if (f != NULL) {
            fseek(f, 0L, SEEK_END);
            file_size = ftell(f);
            buf = malloc((file_size + 1) * sizeof(char));
            rewind(f);
            fread(buf, file_size, 1, f);
            buf[file_size] = '\0';

            vertex_shader = glCreateShader(GL_VERTEX_SHADER);
            glShaderSource(vertex_shader, 1, (const GLchar *const *)buf, NULL);
            glCompileShader(vertex_shader);

            GLint vertex_compiled;
            glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &vertex_compiled);
            if (vertex_compiled != GL_TRUE) {
                GLsizei log_length = 0;
                GLchar message[1024];
                glGetShaderInfoLog(vertex_shader, 1024, &log_length, message);
                printf("[ERROR] Vertex Shader:\n");
                printf("%s\n", message);
                vertex_success = false;
            } else {
                vertex_success = true;
            }
        } else {
            printf("Couldn't load '%s'\n", vertex_path);
            vertex_success = false;
        }
        free(buf);
    }

    {
        f = fopen(fragment_path, "r");
        if (f != NULL) {

            fseek(f, 0L, SEEK_END);
            file_size = ftell(f);
            buf = malloc((file_size + 1) * sizeof(char));
            rewind(f);
            fread(buf, file_size, 1, f);
            buf[file_size] = '\0';

            fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
            glShaderSource(fragment_shader, 1, (const GLchar *const *)buf,
                           NULL);
            glCompileShader(fragment_shader);

            GLint fragment_compiled;
            glGetShaderiv(fragment_shader, GL_COMPILE_STATUS,
                          &fragment_compiled);
            if (fragment_compiled != GL_TRUE) {
                GLsizei log_length = 0;
                GLchar message[1024];
                glGetShaderInfoLog(fragment_shader, 1024, &log_length, message);
                printf("[ERROR] fragment Shader:\n");
                printf("%s\n", message);
                fragment_success = false;
            } else {
                fragment_success = true;
            }
        } else {
            printf("Couldn't load '%s'\n", fragment_path);
            fragment_success = false;
        }
        free(buf);
    }

    GLint shader_program = 0;
    if ((!fragment_success || !vertex_success))
        return 0;
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    glLinkProgram(shader_program);

    GLint program_linked;
    glGetProgramiv(shader_program, GL_LINK_STATUS, &program_linked);
    if (program_linked != GL_TRUE) {
        GLsizei log_length = 0;
        GLchar message[1024];
        glGetProgramInfoLog(shader_program, 1024, &log_length, message);
        return 0;
    }
    return shader_program;
}

void debug_callback(GLenum source, GLenum type, GLuint id, GLenum severity,
                    GLsizei length, const GLchar *message,
                    const void *userParam) {
    (void)source;
    (void)id;
    (void)length;
    (void)userParam;
    fprintf(stderr,
            "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
            (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""), type,
            severity, message);
}
