#pragma once

#include <vector>
#include <glm/glm.hpp>

#include "shape.hpp"
#include "../gl/texture.hpp"
#include "../color.hpp"
#include "../mesh.hpp"
#include "../platform.h"

namespace cmw::shapes {

class Rectangle: public Shape {
    public:
        Rectangle(gl::Texture2d &texture, Colorf color = colors::White): Shape(texture, color) { }

        Rectangle(const std::vector<Position> &positions, gl::Texture2d &texture, Colorf color = colors::White):
                Rectangle(texture, color) {
            std::vector<Mesh::Vertex> vertices;
            vertices.reserve(4);
            for (std::size_t i = 0; i <= 4; ++i)
                vertices.emplace_back(positions[i]);
            this->mesh.set_data(vertices, {0, 1, 2, 2, 3, 0});
        }

        Rectangle(const std::vector<Mesh::Vertex> &vertices, gl::Texture2d &texture, Colorf color = colors::White):
                Rectangle(texture, color) {
            this->mesh.set_data(vertices, {0, 1, 2, 2, 3, 0});
        }

        void edit_point(std::size_t idx, const Position &position) {
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
