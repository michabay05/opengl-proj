#ifndef _RENDERER_H_
#define _RENDERER_H_

#include <stdbool.h>

struct Shader {
    int id;
};

bool shader_load_file(const char *vertex_path, const char *fragment_path, struct Shader *sh);

#endif // _RENDERER_H_
