#ifndef _RENDERER_H_
#define _RENDERER_H_

#include "common.h"
#include "cglm/types-struct.h"
#include "gl_objects.h"
#include "shader.h"

struct VertexInfo {
    vec3s pos;
    vec4s color;
};

#define MAX_VERTEX_COUNT 10000

struct Renderer {
    struct VertexInfo vertices[MAX_VERTEX_COUNT];
    uint32_t vertices_count;
    uint32_t width;
    uint32_t height;
    struct Shader shader;

    // uint32_t vao;
    // uint32_t vbo;
    struct VAO vao;
    struct VBO vbo;

    bool polygon_mode;
};

extern struct Renderer rend;

bool renderer_init(uint32_t width, uint32_t height);
void renderer_push_vertex(vec3s pos, vec4s color);
void renderer_mode(bool line_mode);

#endif // _RENDERER_H
