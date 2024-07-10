#version 330 core

in vec3 position;
in vec4 color;

out vec4 frag_color;

void main() {
    gl_Position = position;
    frag_color = color;
}
