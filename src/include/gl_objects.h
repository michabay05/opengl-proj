#ifndef _GL_OBJECTS_H_
#define _GL_OBJECTS_H_

#include "common.h"
#include "glad/glad.h"

struct VAO {
    GLuint handle;
};

enum VBOType {
    VBOT_Array,
    VBOT_Element,
};

struct VBO {
    GLuint handle;
    enum VBOType type;
    bool is_dynamic;
};

void vao_create(struct VAO *vao);
void vao_bind(struct VAO vao);
void vao_unbind(void);
void vao_destroy(struct VAO vao);
void vao_attr(void);

void vbo_create(struct VBO *vbo);
void vbo_bind(struct VBO *vbo, enum VBOType type, bool is_dynamic, void *data, int size);
void vbo_unbind(struct VBO vbo);
void vbo_destroy(struct VBO vbo);

#endif // _GL_OBJECTS_H_
