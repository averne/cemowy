// Copyright (C) 2019 averne
//
// This file is part of cemowy.
//
// cemowy is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// cemowy is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with cemowy.  If not, see <http://www.gnu.org/licenses/>.

#pragma once

#include <unordered_map>
#include <tuple>
#include <memory>
#include <stb_truetype.h>

#include "cmw/gl/texture.hpp"
#include "cmw/utils/color.hpp"
#include "cmw/platform.h"

namespace cmw {

class Glyph {
    public:
        Glyph(stbtt_fontinfo *font_ctx, float scale, int codepoint, int idx, void *data, int width, int height, int off_x, int off_y);

        inline void bind() const {
            this->texture.bind();
        }

        inline int get_codepoint() const { return this->codepoint; }
        inline int get_idx()       const { return this->idx; }

        inline gl::Texture2d &get_texture() { return this->texture; }
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

        inline bool has_glyph(char16_t chr) const {
            return this->cached_glyphs.count(chr) || stbtt_FindGlyphIndex(&this->font_ctx, chr);
        }

        Glyph &cache_glyph(char16_t chr);

        Glyph &get_glyph(char16_t chr);

        inline stbtt_fontinfo *get_ctx() { return &this->font_ctx; }

        inline int get_ascender()  const { return this->ascender; }
        inline int get_descender() const { return this->descender; }
        inline int get_linegap()   const { return this->linegap; }

        inline int get_kerning(int ch1, int ch2) const {
            return stbtt_GetCodepointKernAdvance(&this->font_ctx, ch1, ch2);
        }

        static constexpr float get_font_scale() { return font_scale; }

    protected:
        static constexpr float font_scale = 0.105f;

        std::vector<std::uint8_t> data;
        stbtt_fontinfo font_ctx;
        int ascender, descender, linegap;
        std::unordered_map<char16_t, Glyph> cached_glyphs;
#ifdef CMW_SWITCH
        PlFontData font_data{};
#endif
};

} // namespace cmw
