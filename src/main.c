#include "cglm/types.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "window.h"
#include "shader.h"

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
void init_buffers(VertInfo *data, GLuint *vao, GLuint *vbo);

int main(void)
{
    if (!window_create(800, 600, "OpenGL Project")) {
        return 1;
    }

    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(debug_callback, NULL);

    glViewport(0, 0, 800, 600);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);

    GLuint vao, vbo;
    VertInfo triangle[] = {
        {{-0.5, -0.5, 0.0}, {1.0, 0.0, 0.0, 1.0}},
        {{0.5, -0.5, 0.0}, {0.0, 1.0, 0.0, 1.0}},
        {{0.0, 0.5, 0.0}, {0.0, 0.0, 1.0, 1.0}},
    };
    init_buffers(triangle, &vao, &vbo);

    struct Shader shader;
    bool success = shader_load_file("res/shaders/circle.vert", "res/shaders/circle.frag", &shader);
    if (!success) {
        printf("[ERROR] Shader compilation failed.\n");
    }
    glUseProgram(shader.id);

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

void init_buffers(VertInfo *data, GLuint *vao, GLuint *vbo)
{
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

void debug_callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
                    const GLchar *message, const void *userParam)
{
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
