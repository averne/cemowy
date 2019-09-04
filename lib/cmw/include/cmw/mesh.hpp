#pragma once

#include <vector>
#include <glm/glm.hpp>

#include "gl/buffer.hpp"
#include "gl/vertex_array.hpp"
#include "utils.hpp"
#include "platform.h"

namespace cmw {

class Mesh {
    public:
        struct Vertex {
            glm::vec3 position;
            glm::vec2 uv;
        };

        Mesh()  {
            this->vao.bind();
            this->vbo.set_layout({
                gl::BufferElement::Float3,
                gl::BufferElement::Float2,
            });
        }

        ~Mesh() = default;

        void fill_buffers(std::vector<Vertex> &vertices) {
            this->vbo.bind();
            this->vbo.set_data(vertices.data(), sizeof(Vertex) * vertices.size());
            this->size = vertices.size();
        }

        inline void bind() const { this->vao.bind(); }

        inline const gl::VertexBuffer &get_vertices() const { return this->vbo; }

        inline std::size_t get_size() const { return this->size; }

    protected:
        std::size_t size = 0;
        gl::VertexArray vao;
        gl::VertexBuffer vbo;
};

} // namespace cmw
