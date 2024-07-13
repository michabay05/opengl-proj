#ifndef _SHADER_H_
#define _SHADER_H_

#include "common.h"

struct Shader {
    int32_t id;
};

bool shader_load_file(const char *vertex_path, const char *fragment_path, struct Shader *sh);
void shader_bind(struct Shader sh);
void shader_unbind(void);

#endif // _SHADER_H_
