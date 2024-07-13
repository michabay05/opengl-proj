#include "cglm/types.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "renderer.h"
#include "window.h"

#include <stddef.h>
#include <stdio.h>
#include <stdbool.h>

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
        printf("[ERROR] Failed to create window.\n");
        return 1;
    }

    if (!renderer_init(window.width, window.height)) {
        printf("[ERROR] Failed to init renderer.\n");
        return 1;
    }

    while (!glfwWindowShouldClose(window.handle)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.11, 0.11, 0.11, 1.0);

        glBindVertexArray(rend.vao.handle);
        glBindBuffer(GL_ARRAY_BUFFER, rend.vbo.handle);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window.handle);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
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
