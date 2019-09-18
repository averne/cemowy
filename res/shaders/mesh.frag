#version 430 core

#define MAX_TEXTURES 30

#define MODE_DEFAULT    0
#define MODE_ALPHAMAP   1

in DATA {
   vec2 uv;
   vec4 blend_color;
   flat int tex_idx;
   flat uint mode;
} f_in;

out vec4 color;

uniform sampler2D u_textures[MAX_TEXTURES];

void main() {
    if (f_in.mode == MODE_ALPHAMAP)
        color = vec4(f_in.blend_color.rgb, texture(u_textures[f_in.tex_idx], f_in.uv).r);
    else
        color = texture(u_textures[f_in.tex_idx], f_in.uv) * f_in.blend_color;
}
