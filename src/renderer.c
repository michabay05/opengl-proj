#include "include/renderer.h"
#include "gl_objects.h"
#include "glad/glad.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Renderer rend;

static void _debug_callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
                            const GLchar *message, const void *userParam);
static void _init_buffers_glo(struct VertexInfo *vert, uint32_t vert_count, struct VAO *vao,
                              struct VBO *vbo);

bool renderer_init(uint32_t width, uint32_t height)
{
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(_debug_callback, NULL);

    glViewport(0, 0, width, height);
    rend.width = width;
    rend.height = height;

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);

    struct VertexInfo triangle[] = {
        {{{-0.5, -0.5, 0.0}}, {{1.0, 0.0, 0.0, 1.0}}},
        {{{0.5, -0.5, 0.0}}, {{0.0, 1.0, 0.0, 1.0}}},
        {{{0.0, 0.5, 0.0}}, {{0.0, 0.0, 1.0, 1.0}}},
    };
    renderer_push_vertex(triangle[0].pos, triangle[0].color);
    renderer_push_vertex(triangle[1].pos, triangle[1].color);
    renderer_push_vertex(triangle[2].pos, triangle[2].color);

    _init_buffers_glo(rend.vertices, rend.vertices_count, &rend.vao, &rend.vbo);

    bool success =
        shader_load_file("res/shaders/circle.vert", "res/shaders/circle.frag", &rend.shader);
    if (!success) {
        printf("[ERROR] Shader compilation failed.\n");
    }

    shader_bind(rend.shader);

    return true;
}

void renderer_mode(bool line_mode)
{
    if (line_mode)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    else
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void renderer_push_vertex(vec3s pos, vec4s color)
{
    rend.vertices[rend.vertices_count] = (struct VertexInfo){
        .pos = pos,
        .color = color,
    };
    rend.vertices_count++;
}

static void _init_buffers_glo(struct VertexInfo *vert, uint32_t vert_count, struct VAO *vao,
                              struct VBO *vbo)
{
    vao_create(vao);
    vao_bind(*vao);

    vbo_create(vbo);
    vbo_bind(vbo, VBOT_Array, false, (void*)vert, vert_count * sizeof(vert[0]));

    int32_t pos_loc = 0;
    glVertexAttribPointer(pos_loc, 3, GL_FLOAT, GL_FALSE, sizeof(struct VertexInfo),
                          (void *)offsetof(struct VertexInfo, pos));
    glEnableVertexAttribArray(pos_loc);

    int32_t color_loc = 1;
    glVertexAttribPointer(color_loc, 4, GL_FLOAT, GL_FALSE, sizeof(struct VertexInfo),
                          (void *)offsetof(struct VertexInfo, color));
    glEnableVertexAttribArray(color_loc);

    vbo_unbind(*vbo);
    vao_unbind();
}

static void _debug_callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
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
    printf("[%s] GL CALLBACK: type = 0x%x, severity = %s, message = %s\n",
           (type == GL_DEBUG_TYPE_ERROR ? "GL ERROR" : ""), type, severity_str, message);
}
