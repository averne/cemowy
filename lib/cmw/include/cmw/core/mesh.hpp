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

#include <cstdint>
#include <vector>
#include <glm/glm.hpp>

#include "cmw/gl/buffer.hpp"
#include "cmw/gl/texture.hpp"
#include "cmw/gl/vertex_array.hpp"
#include "cmw/utils/color.hpp"
#include "cmw/utils/position.hpp"
#include "cmw/utils.hpp"

namespace cmw {

class Mesh {
    public:
        struct Vertex {
            Position position;
            Position2f uv;

            constexpr Vertex(const Position &position): position(position), uv(Position2f(0.5f, 0.5f)) { }
            constexpr Vertex(const Position &position, const Position2f &uv): position(position), uv(uv) { }
        };

        using Index = std::uint32_t;

    public:
        Mesh(gl::Texture2d &texture, Colorf color = colors::White): texture(texture), blend_color(color)  { }

        Mesh(const std::vector<Vertex> &vertices, gl::Texture2d &texture, Colorf color = colors::White): Mesh(texture, color) {
            set_data(vertices);
        }

        Mesh(const std::vector<Vertex> &vertices, const std::vector<Index> &indices, gl::Texture2d &texture, Colorf color = colors::White):
                Mesh(texture, color) {
            set_data(vertices, indices);
        }

        inline std::vector<Vertex>       &get_vertices()       { return this->vertices; }
        inline const std::vector<Vertex> &get_vertices() const { return this->vertices; }
        inline std::vector<Index>        &get_indices()        { return this->indices; }
        inline const std::vector<Index>  &get_indices()  const { return this->indices; }
        inline void set_data(const std::vector<Vertex> &vertices) { this->vertices = vertices; }
        inline void set_data(const std::vector<Index> &indices)   { this->indices = indices; }
        inline void set_data(const std::vector<Vertex> &vertices, const std::vector<Index> &indices) { this->vertices = vertices; this->indices = indices; }

        inline       gl::Texture2d &get_texture()       { return this->texture; }
        inline const gl::Texture2d &get_texture() const { return this->texture; }

        inline bool is_indexed() const { return !!this->indices.size(); }
        inline std::size_t get_size() const { return std::max(this->vertices.size(), this->indices.size()); }

        inline Colorf get_blend_color() const { return this->blend_color; }
        inline void set_blend_color(Colorf color) { this->blend_color = color; }

    protected:
        gl::Texture2d &texture;

        std::vector<Vertex> vertices;
        std::vector<Index> indices;
        Colorf blend_color;
};

} // namespace cmw
