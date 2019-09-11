#pragma once

#include <unordered_map>
#include <tuple>
#include <memory>
#include <stb_truetype.h>

#include <cmw/gl/texture.hpp>
#include <cmw/gl/buffer.hpp>
#include <cmw/gl/vertex_array.hpp>
#include <cmw/gl/shader_program.hpp>
#include <cmw/color.hpp>
#include <cmw/window.hpp>
#include <cmw/platform.h>

namespace cmw {

class Glyph {
    public:
        Glyph(stbtt_fontinfo *font_ctx, float scale, int codepoint, int idx, void *data, int width, int height, int off_x, int off_y);
        ~Glyph() = default;

        inline void bind() const {
            this->texture.bind();
        }

        inline int get_codepoint() const { return this->codepoint; }
        inline int get_idx()       const { return this->idx; }

        inline const gl::Texture2d &get_texture() const { return this->texture; }
        inline int get_width()      const { return this->x2 - this->x1; }
        inline int get_height()     const { return this->y2 - this->y1; }
        inline int get_bitmap_top() const { return this->y1; }
        inline int get_advance()    const { return this->advance; }
        inline int get_bearing()    const { return this->bearing; }

    protected:
        gl::Texture2d texture;
        int codepoint, idx;
        int buf_width, buf_height, buf_off_x, buf_off_y;
        int x1, y1, x2, y2;
        int advance, bearing;
};

class Font {
    public:
        using Bbox = std::tuple<int, int, int, int>;

#ifdef CMW_SWITCH
        Font(PlSharedFontType type, char16_t first_cached = ' ', char16_t last_cached = '~');
#endif

        Font(void *data,              char16_t first_cached = ' ', char16_t last_cached = '~');
        Font(const std::string &path, char16_t first_cached = ' ', char16_t last_cached = '~');
        ~Font();

        const Glyph &cache_glyph(char16_t chr);

        const Glyph &get_glyph(char16_t chr);

        inline void bind() const {
            bind_all(this->vao, this->vbo);
        }

        inline void set_vertex_data(void *vertices, std::size_t size) const {
            this->vbo.set_sub_data(vertices, size);
        }

        inline stbtt_fontinfo *get_ctx() { return &this->font_ctx; }

        inline int get_ascender()  const { return this->ascender; }
        inline int get_descender() const { return this->descender; }
        inline int get_linegap()   const { return this->linegap; }

        static constexpr float get_font_scale() { return font_scale; }

    protected:
        static constexpr float font_scale = 0.105f;

        gl::VertexArray vao;
        gl::VertexBuffer vbo;

        unsigned char *data = nullptr;
        stbtt_fontinfo font_ctx;
        int ascender, descender, linegap;
        std::unordered_map<char16_t, Glyph> cached_glyphs;
#ifdef CMW_SWITCH
        PlFontData font_data{};
#endif
};

} // namespace cmw
