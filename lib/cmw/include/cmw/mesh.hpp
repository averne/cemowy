#pragma once

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

        Mesh(gl::Texture2d &texture, Colorf color = colors::White): texture(texture), blend_color(color)  {
            this->vao.bind();
            this->vbo.set_layout({
                gl::BufferElement::Float3,
                gl::BufferElement::Float2,
            });
        }

        Mesh(const std::vector<Vertex> &vertices, gl::Texture2d &texture, Colorf color = colors::White): Mesh(texture, color) {
            this->vertices = vertices;
            fill_buffers();
        }

        ~Mesh() = default;

        void fill_buffers() {
            this->vbo.bind();
            this->vbo.set_data(this->vertices.data(), sizeof(Vertex) * this->vertices.size());
        }

        inline std::vector<Vertex> &get_vertices() { return this->vertices; }
        inline void set_vertices(const std::vector<Vertex> &vertices) { this->vertices = vertices; fill_buffers(); }
        inline std::size_t get_size() const { return this->vertices.size(); }

        inline Colorf get_blend_color() const { return this->blend_color; }
        inline void set_blend_color(Colorf color) { this->blend_color = color; }

        inline void bind() const { bind_all(this->vao, this->vbo, this->texture); }
        inline const gl::VertexBuffer &get_vertex_buffer() const { return this->vbo; }

    protected:
        gl::VertexArray vao;
        gl::VertexBuffer vbo;
        gl::Texture2d &texture;

        std::vector<Vertex> vertices;
        Colorf blend_color;
};

} // namespace cmw
