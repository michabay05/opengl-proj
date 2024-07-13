#include "shader.h"

#include "glad/glad.h"
#include <stdio.h>
#include <stdlib.h>

static bool _compile_shader(const char *path, bool is_vertex, int *shader);
static bool _link_program(int vertex_id, int fragment_id, int *program);

bool shader_load_file(const char *vertex_path, const char *fragment_path, struct Shader *sh)
{
    int vertex_shader, fragment_shader;
    bool vertex_success = _compile_shader(vertex_path, true, &vertex_shader);
    bool fragment_success = _compile_shader(fragment_path, false, &fragment_shader);

    if (!fragment_success || !vertex_success) {
        printf("[ERROR] Vertex success: %d\n", vertex_success);
        printf("[ERROR] Fragment success: %d\n", fragment_success);
        return false;
    }

    return _link_program(vertex_shader, fragment_shader, &sh->id);
}

void shader_bind(struct Shader sh)
{
    glUseProgram(sh.id);
}

void shader_unbind(void)
{
    glUseProgram(0);
}

static bool _compile_shader(const char *path, bool is_vertex, int *shader)
{
    FILE *f = fopen(path, "r");
    if (f == NULL) {
        return false;
    }
    fseek(f, 0L, SEEK_END);
    long file_size = ftell(f);
    rewind(f);
    char *buf = malloc(sizeof(char) * (file_size + 1));
    fread(buf, file_size, 1, f);
    buf[file_size] = '\0';

    int sh_id = glCreateShader(is_vertex ? GL_VERTEX_SHADER : GL_FRAGMENT_SHADER);
    glShaderSource(sh_id, 1, (const GLchar *const *)&buf, NULL);
    glCompileShader(sh_id);

    GLint success;
    glGetShaderiv(sh_id, GL_COMPILE_STATUS, &success);
    if (success != GL_TRUE) {
        GLsizei log_length = 0;
        GLchar message[1024];
        glGetShaderInfoLog(sh_id, 1024, &log_length, message);
        printf("[ERROR] %s shader:\n", is_vertex ? "Vertex" : "Fragment");
        printf("%s\n", message);
        return false;
    }

    free(buf);
    *shader = sh_id;
    return true;
}

static bool _link_program(int vertex_id, int fragment_id, int *program)
{
    int shader_program = glCreateProgram();

    glAttachShader(shader_program, vertex_id);
    glAttachShader(shader_program, fragment_id);
    glLinkProgram(shader_program);

    GLint program_linked;
    glGetProgramiv(shader_program, GL_LINK_STATUS, &program_linked);
    if (program_linked != GL_TRUE) {
        GLsizei log_length = 0;
        GLchar message[1024];
        glGetProgramInfoLog(shader_program, 1024, &log_length, message);
        printf("[ERROR] Shader program link:\n\t%s\n", message);
        return false;
    }

    glDeleteShader(vertex_id);
    glDeleteShader(fragment_id);
    *program = shader_program;
    return true;
}
