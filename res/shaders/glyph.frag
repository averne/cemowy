#version 330 core

in vec2 tex_coords;

out vec4 color;

uniform sampler2D tex;
uniform vec3 glyph_color;

void main() {
    color = vec4(glyph_color, texture(tex, tex_coords).r);
}
