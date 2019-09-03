#version 330 core

in vec2 tex_coords;

out vec4 color;

uniform sampler2D u_texture;
uniform vec4 u_blend_color;

void main() {
    color = texture(u_texture, tex_coords) * u_blend_color;
}
