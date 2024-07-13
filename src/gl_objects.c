#include "gl_objects.h"

void vao_create(struct VAO *vao)
{
    glGenVertexArrays(1, &vao->handle);
}

void vao_bind(struct VAO vao)
{
    glBindVertexArray(vao.handle);
}

void vao_unbind(void)
{
    glBindVertexArray(0);
}

void vao_destroy(struct VAO vao)
{
    glDeleteVertexArrays(1, &vao.handle);
}

void vbo_create(struct VBO *vbo)
{
    glGenBuffers(1, &vbo->handle);
}

void vbo_bind(struct VBO *vbo, enum VBOType type, bool is_dynamic, void *data, int size)
{
    vbo->type = type;
    vbo->is_dynamic = is_dynamic;
    GLuint gl_type = vbo->type == VBOT_Array ? GL_ARRAY_BUFFER : GL_ELEMENT_ARRAY_BUFFER;
    glBindBuffer(gl_type, vbo->handle);
    glBufferData(gl_type, size, data, is_dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
}

void vbo_unbind(struct VBO vbo)
{
    GLuint type = vbo.type == VBOT_Array ? GL_ARRAY_BUFFER : GL_ELEMENT_ARRAY_BUFFER;
    glBindBuffer(type, 0);
}

void vbo_destroy(struct VBO vbo)
{
    glDeleteBuffers(1, &vbo.handle);
}
