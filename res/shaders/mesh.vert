#version 430 core

layout (location = 0) in vec3 in_position;
layout (location = 1) in vec2 in_uv;
layout (location = 2) in vec4 in_blend_color;
layout (location = 3) in int  in_tex_idx;
layout (location = 4) in uint in_mode;

out DATA {
   vec2      uv;
   vec4      blend_color;
   flat int  tex_idx;
   flat uint mode;
} v_out;

uniform mat4 u_view_proj;

void main() {
    v_out.uv          = in_uv;
    v_out.blend_color = in_blend_color;
    v_out.tex_idx     = in_tex_idx;
    v_out.mode        = in_mode;
    gl_Position = u_view_proj * vec4(in_position, 1.0f);
}
