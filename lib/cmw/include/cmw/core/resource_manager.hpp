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

#include <cstdio>
#include <cstring>
#include <string>
#include <map>

#include "cmw/core/log.hpp"
#include "cmw/platform.h"

namespace cmw {

// Forward declarations
namespace gl {
    class ShaderProgram;

    template <std::size_t N>
    class Texture2dN;
    using Texture2d = Texture2dN<1>;
}

class ResourceManager {
    public:
        static inline std::string WhiteTexture = "";

        ResourceManager();

        gl::Texture2d &get_texture(const std::string &path);
        gl::ShaderProgram &get_shader(const std::string &vert_path, const std::string &frag_path);

        gl::Texture2d &get_white_texture() const { return *this->white_texture; }

        static inline FILE *open_asset(const std::string &path, const std::string &mode = "r") {
#ifdef CMW_SWITCH
            std::string asset_path = "romfs:/" + path;
#else
            std::string asset_path = "res/"    + path;
#endif
            FILE *fp = fopen(asset_path.c_str(), mode.c_str());
            if (!fp)
                CMW_ERROR("Failed to open %s\n", asset_path.c_str());
            CMW_TRACE("Loaded %s\n", asset_path.c_str());
            return fp;
        }

        template <typename T>
        static inline T read_asset(const std::string &path) {
            FILE *fp = open_asset(path);
            fseek(fp, 0, SEEK_END);
            std::size_t size = ftell(fp);
            fseek(fp, 0, SEEK_SET);

            T container;
            container.reserve(size);
            if (fread(container.data(), 1, size, fp) != size)
                CMW_ERROR("Failed to read %s\n", path.c_str());
            container[size] = 0; // Ensure null termination

            return container;
        }

    private:
        gl::Texture2d *white_texture;
        std::map<std::string, gl::Texture2d> textures;
        std::map<std::string, gl::ShaderProgram> shader_programs;
};

} // namespace cmw
