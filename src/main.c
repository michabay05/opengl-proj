#include "cglm/types.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "window.h"

#include <stddef.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    vec3 pos;
    vec4 color;
} VertInfo;

void debug_callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
                    const GLchar *message, const void *userParam);
GLint init_shaders(const char *vertex_path, const char *frag_path);
void init_buffers(VertInfo *data, GLuint *vao, GLuint *vbo);

int main(void) {
    if (!window_create(800, 600, "OpenGL Project")) {
        return 1;
    }

    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(debug_callback, NULL);

    glViewport(0, 0, 800, 600);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);
    // TODO: implement frame resize callback

    GLuint vao, vbo;
    VertInfo triangle[] = {
        {{-0.5, -0.5, 0.0}, {1.0, 0.0, 0.0, 1.0}},
        {{0.5, -0.5, 0.0}, {0.0, 1.0, 0.0, 1.0}},
        {{0.0, 0.5, 0.0}, {0.0, 0.0, 1.0, 1.0}},
    };
    init_buffers(triangle, &vao, &vbo);

    GLuint shader = init_shaders("res/shaders/circle.vert", "res/shaders/circle.frag");
    if (shader == 0) {
        printf("[ERROR] Shader compilation failed.\n");
    }
    glUseProgram(shader);

    while (!glfwWindowShouldClose(window.handle)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.11, 0.11, 0.11, 1.0);

        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window.handle);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void init_buffers(VertInfo *data, GLuint *vao, GLuint *vbo) {
    glGenVertexArrays(1, vao);
    glBindVertexArray(*vao);

    glGenBuffers(1, vbo);
    glBindBuffer(GL_ARRAY_BUFFER, *vbo);
    glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(data[0]), data, GL_STATIC_DRAW);

    GLint pos_loc = 0;
    glVertexAttribPointer(pos_loc, 3, GL_FLOAT, GL_FALSE, sizeof(VertInfo),
                          (void *)offsetof(VertInfo, pos));
    glEnableVertexAttribArray(pos_loc);

    GLint color_loc = 1;
    glVertexAttribPointer(color_loc, 4, GL_FLOAT, GL_FALSE, sizeof(VertInfo),
                          (void *)offsetof(VertInfo, color));
    glEnableVertexAttribArray(color_loc);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
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
            buf = malloc(sizeof(char) * (file_size + 1));
            rewind(f);
            fread(buf, file_size, 1, f);
            buf[file_size] = '\0';

            vertex_shader = glCreateShader(GL_VERTEX_SHADER);
            glShaderSource(vertex_shader, 1, (const GLchar *const *)&buf, NULL);
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
            buf = malloc(sizeof(char) * (file_size + 1));
            rewind(f);
            fread(buf, file_size, 1, f);
            buf[file_size] = '\0';

            fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
            glShaderSource(fragment_shader, 1, (const GLchar *const *)&buf, NULL);
            glCompileShader(fragment_shader);

            GLint fragment_compiled;
            glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &fragment_compiled);
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

    GLint shader_program = glCreateProgram();
    if (!fragment_success || !vertex_success) {
        printf("[ERROR] Vertex success: %d\n", vertex_success);
        printf("[ERROR] Fragment success: %d\n", fragment_success);
        return 0;
    }
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
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
    return shader_program;
}

void debug_callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
                    const GLchar *message, const void *userParam) {
    (void)source;
    (void)id;
    (void)length;
    (void)userParam;
    const char *severity_str;
    switch (severity) {
    case GL_DEBUG_SEVERITY_HIGH:
        severity_str = "high";
        break;
    case GL_DEBUG_SEVERITY_MEDIUM:
        severity_str = "medium";
        break;
    case GL_DEBUG_SEVERITY_LOW:
        severity_str = "low";
        break;
    default:
        severity_str = "unknown";
        break;
    }
    fprintf(stderr, "[%s] GL CALLBACK: type = 0x%x, severity = %s, message = %s\n",
            (type == GL_DEBUG_TYPE_ERROR ? "GL ERROR" : ""), type, severity_str, message);
}
