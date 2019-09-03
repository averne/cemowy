#version 330 core

layout (location = 0) in vec3 in_position;
layout (location = 1) in vec2 in_uvs;

out vec2 tex_coords;

uniform mat4 u_mvp_mat;

void main() {
    tex_coords = in_uvs;
    gl_Position = u_mvp_mat * vec4(in_position, 1.0f);
}
