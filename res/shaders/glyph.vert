#version 330 core

layout (location = 0) in vec3 in_position;
layout (location = 1) in vec2 in_uv;

out vec2 tex_coords;

uniform mat4 u_view_proj_mat;

void main() {
    tex_coords = in_uv;
    gl_Position = u_view_proj_mat * vec4(in_position, 1.0);
}
