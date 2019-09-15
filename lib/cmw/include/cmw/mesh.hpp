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

#include "gl/buffer.hpp"
#include "gl/texture.hpp"
#include "gl/vertex_array.hpp"
#include "color.hpp"
#include "position.hpp"
#include "utils.hpp"
#include "platform.h"

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
        Mesh(gl::Texture2d &texture, Colorf color = colors::White): texture(texture), blend_color(color)  {
            bind_all(this->vao, this->vbo, this->ebo);
            this->vbo.set_layout({
                gl::BufferElement::Float3,
                gl::BufferElement::Float2,
            });
        }

        Mesh(const std::vector<Vertex> &vertices, gl::Texture2d &texture, Colorf color = colors::White): Mesh(texture, color) {
            set_data(vertices);
        }

        Mesh(const std::vector<Vertex> &vertices, const std::vector<Index> &indices, gl::Texture2d &texture, Colorf color = colors::White):
                Mesh(texture, color) {
            set_data(vertices, indices);
        }

        void fill_buffers() {
            bind_all(this->vbo, this->ebo);
            this->vbo.set_data(this->vertices.data(), sizeof(Vertex) * this->vertices.size());
            if (is_indexed())
                this->ebo.set_data(this->indices.data(), sizeof(Index) * this->indices.size());
        }

        inline std::vector<Vertex> &get_vertices() { return this->vertices; }
        inline std::vector<Index>  &get_indices()  { return this->indices; }
        inline void set_data(const std::vector<Vertex> &vertices) { this->vertices = vertices; fill_buffers(); }
        inline void set_data(const std::vector<Index> &indices)   { this->indices = indices;   fill_buffers(); }
        inline void set_data(const std::vector<Vertex> &vertices, const std::vector<Index> &indices)
            { this->vertices = vertices; this->indices = indices; fill_buffers(); }

        inline bool is_indexed() const { return !!this->indices.size(); }
        inline std::size_t get_size() const { return std::max(this->vertices.size(), this->indices.size()); }

        inline Colorf get_blend_color() const { return this->blend_color; }
        inline void set_blend_color(Colorf color) { this->blend_color = color; }

        inline void bind() const { bind_all(this->vao, this->texture); }
        inline const gl::VertexBuffer &get_vertex_buffer() const { return this->vbo; }

    protected:
        gl::VertexArray vao;
        gl::ElementBuffer ebo;
        gl::VertexBuffer vbo;
        gl::Texture2d &texture;

        std::vector<Vertex> vertices;
        std::vector<Index> indices;
        Colorf blend_color;
};

} // namespace cmw
