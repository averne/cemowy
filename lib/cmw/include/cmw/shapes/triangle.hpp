#pragma once

#include <vector>
#include <glm/glm.hpp>

#include "shape.hpp"
#include "../gl/texture.hpp"
#include "../color.hpp"
#include "../mesh.hpp"
#include "../platform.h"

namespace cmw::shapes {

class Triangle: public Shape {
    public:
        Triangle(gl::Texture2d &texture, Colorf color = colors::White): Shape(texture, color) { }

        Triangle(const std::vector<Mesh::Vertex> &vertices, gl::Texture2d &texture, Colorf color = colors::White):
            Shape(vertices, texture, color) { }

        Triangle(const std::vector<glm::vec3> &positions, gl::Texture2d &texture, Colorf color = colors::White):
                Triangle(texture, color) {
            std::vector<Mesh::Vertex> vertices;
            vertices.reserve(3);
            for (std::size_t i = 0; i <= 3; ++i)
                vertices.emplace_back(positions[i]);
            this->mesh.set_data(vertices);
        }

        void edit_point(std::size_t idx, const glm::vec3 &position) {
            this->mesh.get_vertices()[idx].position = position;
            this->mesh.fill_buffers();
        }

        void edit_point(std::size_t idx, const Mesh::Vertex &vertex) {
            this->mesh.get_vertices()[idx] = vertex;
            this->mesh.fill_buffers();
        }

        virtual void on_draw() override { }
};

} // namespace cmw::shapes
