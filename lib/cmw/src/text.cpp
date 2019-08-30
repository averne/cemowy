#include <cstdio>
#include <stdexcept>
#include <stb_truetype.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <cmw/gl/shader_program.hpp>
#include <cmw/window.hpp>
#include <cmw/error.hpp>
#include <cmw/utils.hpp>
#include <cmw/platform.h>

#include <cmw/text.hpp>

namespace cmw {

Glyph::Glyph(stbtt_fontinfo *font_ctx, float scale, int codepoint, int idx, void *data, int width, int height, int off_x, int off_y):
        codepoint(codepoint), idx(idx), buf_width(width), buf_height(height), buf_off_x(off_x), buf_off_y(off_y) {
    texture.set_data(data, width, height, GL_RED, GL_RED);
    texture.set_parameters(
        std::pair{GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE},
        std::pair{GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE},
        std::pair{GL_TEXTURE_MIN_FILTER, GL_LINEAR},
        std::pair{GL_TEXTURE_MAG_FILTER, GL_LINEAR}
    );
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    stbtt_GetGlyphBitmapBox(font_ctx, idx, scale, scale, &this->x1, &this->y1, &this->x2, &this->y2);
    stbtt_GetGlyphHMetrics(font_ctx, idx, &this->advance, &this->bearing);
    this->advance *= scale; this->bearing *= scale;
}

#define INIT_FONT(data) ({                                                                                          \
    CMW_TRY_THROW(stbtt_InitFont(&this->font_ctx, (unsigned char *)data, 0),                                        \
        std::runtime_error("Failed to initialize font"));                                                           \
                                                                                                                    \
    stbtt_GetFontVMetrics(&this->font_ctx, &this->ascender, &this->descender, &this->linegap);                      \
    this->ascender *= this->font_scale; this->descender *= this->font_scale; this->linegap *= this->font_scale;     \
                                                                                                                    \
    for (char16_t i=first_cached; i<=last_cached; ++i)                                                              \
        cache_glyph(i);                                                                                             \
                                                                                                                    \
    this->program.set_shaders(VertexShader{"shaders/glyph.vert"}, FragmentShader{"shaders/glyph.frag"});            \
    this->program.link();                                                                                           \
                                                                                                                    \
    this->vao.bind();                                                                                               \
    this->vbo.set_data(nullptr, 6 * 4 * sizeof(float), GL_DYNAMIC_DRAW);                                            \
    this->vbo.set_layout({ BufferElement::Float4 });                                                                \
})

#ifdef CMW_SWITCH
Font::Font(PlSharedFontType type, char16_t first_cached, char16_t last_cached) {
    CMW_TRY_RC_THROW(plInitialize(), std::runtime_error("Failed to initialize pl"));
    CMW_TRY_RC_THROW(plGetSharedFontByType(&this->font_data, type), std::runtime_error("Failed to get font"));
    INIT_FONT(this->font_data.address);
}
#endif // CMW_SWITCH

Font::Font(void *data, char16_t first_cached, char16_t last_cached) {
    INIT_FONT(data);
}

Font::Font(const std::string &path, char16_t first_cached, char16_t last_cached) {
    FILE *fp = open_asset(path, "rb");
    CMW_TRY_THROW(fp, std::runtime_error("Failed to open font file"));
    fseek(fp, 0, SEEK_END);
    std::size_t size = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    this->data = new unsigned char[size];
    fread(this->data, size, 1, fp);
    INIT_FONT(this->data);
}

Font::~Font() {
    if (this->data)
        delete[] this->data;
#ifdef CMW_SWITCH
    if (this->font_data.address)
        plExit();
#endif
}

const Glyph &Font::cache_glyph(char16_t chr) {
    int w, h, x, y;
    int idx = stbtt_FindGlyphIndex(&this->font_ctx, chr);
    void *data = stbtt_GetGlyphBitmap(&this->font_ctx, this->font_scale, this->font_scale, idx, &w, &h, &x, &y);
    auto pair = this->cached_glyphs.try_emplace(chr, &this->font_ctx, this->font_scale, chr, idx, data, w, h, x, y);
    stbtt_FreeBitmap((unsigned char *)data, nullptr);
    return pair.first->second;
}

const Glyph &Font::get_glyph(char16_t chr) {
    auto it = this->cached_glyphs.find(chr);
    if (it != this->cached_glyphs.end())
        return it->second;
    return cache_glyph(chr);
}

void Font::draw_string(std::shared_ptr<Window> window, const std::u16string &str, float x, float y, float scale, Colorf color) {
    auto [window_w, window_h] = window->get_size();
    this->program.bind();
    this->program.set_value("u_projection", glm::ortho(0.0f, (float)window_w, 0.0f, (float)window_h));
    this->program.set_value("glyph_color", color.r, color.g, color.b);

    this->vao.bind();
    this->vbo.bind();
    int last_codepoint = -1;
    float xpos = x, ypos = y;
    for (char16_t chr: str) {
        if (chr == u'\n') {
            y += (this->ascender + this->descender + 15.0f) * scale; // ??? this->linegap == 0
            xpos = x;
            continue;
        }

        Texture2d::active(0);
        auto &glyph = this->get_glyph(chr);
        glyph.get_texture().bind();

        float w = (float)glyph.get_width() * scale, h = (float)glyph.get_height() * scale;
        xpos += glyph.get_bearing() * scale;
        ypos = window_h - y - h - glyph.get_bitmap_top() * scale;

        GLfloat vertices[6][4] = {
            { xpos,     ypos + h, 0.0, 0.0 },
            { xpos,     ypos,     0.0, 1.0 },
            { xpos + w, ypos,     1.0, 1.0 },
            { xpos,     ypos + h, 0.0, 0.0 },
            { xpos + w, ypos,     1.0, 1.0 },
            { xpos + w, ypos + h, 1.0, 0.0 },
        };

        this->vbo.set_sub_data(vertices, sizeof(vertices));
        glDrawArrays(GL_TRIANGLES, 0, 6);

        xpos += glyph.get_advance() * scale;
        if (last_codepoint)
            xpos += stbtt_GetCodepointKernAdvance(&this->font_ctx, last_codepoint, glyph.get_codepoint()) * scale;
        last_codepoint = glyph.get_codepoint();
    }
}

} // namespace cmw
