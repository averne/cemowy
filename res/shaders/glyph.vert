#version 330 core

layout (location = 0) in vec4 in_vertex;

out vec2 tex_coords;

uniform mat4 u_projection;

void main() {
    tex_coords = in_vertex.zw;
    gl_Position = u_projection * vec4(in_vertex.xy, 0.0, 1.0);
}
