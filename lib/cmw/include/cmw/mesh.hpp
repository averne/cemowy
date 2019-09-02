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
        Mesh()  {
            this->vao.bind();
            this->vbo.set_layout({
                cmw::BufferElement::Float3,
            });
        }
        ~Mesh() = default;

        void fill_buffers(std::vector<glm::vec3> &vertices) {
            CMW_TRACE("Filling buffer object\n");
            this->vbo.bind();
            this->vbo.set_data(vertices.data(), sizeof(glm::vec3) * vertices.size());
            this->size = vertices.size();
        }

        inline void bind() const { this->vao.bind(); }

        inline const VertexBuffer &get_vertices() const { return this->vbo; }

        inline std::size_t get_size() const { return this->size; }

    protected:
        std::size_t size = 0;
        VertexArray vao;
        VertexBuffer vbo;
};

} // namespace cmw
