#version 330 core

layout (location = 0) in vec3 in_position;
layout (location = 1) in vec2 in_tex_coords;

out vec2 tex_coords;

uniform mat4 view_proj, model;

void main() {
    tex_coords = in_tex_coords;
    gl_Position  = view_proj * model * vec4(in_position, 1.0);
}
