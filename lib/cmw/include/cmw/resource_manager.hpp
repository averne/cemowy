#pragma once

#include "gl/texture.hpp"

namespace cmw {

class ResourceManager {
    public:
        ResourceManager() {
            this->white_tex.set_blank_data(10, 10);
            this->white_tex.generate_mipmap();
        }

        ~ResourceManager() = default;

        const Texture2d &get_white_texture() const { return this->white_tex; }

    private:
        Texture2d white_tex;
};

} // namespace cmw
