#pragma once

#include <cstdint>
#include <vector>
#include <glm/glm.hpp>

#include "gl/buffer.hpp"
#include "gl/texture.hpp"
#include "gl/vertex_array.hpp"
#include "color.hpp"
#include "utils.hpp"
#include "platform.h"

namespace cmw {

class Mesh {
    public:
        struct Vertex {
            glm::vec3 position;
            glm::vec2 uv;

            constexpr Vertex(const glm::vec3 &position): position(position), uv(glm::vec2(0.5f, 0.5f)) { }
            constexpr Vertex(const glm::vec3 &position, const glm::vec2 &uv): position(position), uv(uv) { }
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
