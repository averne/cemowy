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

#include <vector>
#include <glm/glm.hpp>

#include "cmw/utils/color.hpp"
#include "cmw/gl/texture.hpp"
#include "cmw/shapes/shape.hpp"
#include "cmw/core/mesh.hpp"
#include "cmw/utils/position.hpp"

namespace cmw::shapes {

class Triangle: public Shape {
    public:
        Triangle(gl::Texture2d &texture, Colorf color = colors::White): Shape(texture, color) {
            this->mesh.set_data(std::vector<Mesh::Index>{0, 1, 2});
        }

        Triangle(const std::vector<Mesh::Vertex> &vertices, gl::Texture2d &texture, Colorf color = colors::White):
                Shape(vertices, texture, color) {
            this->mesh.set_data(std::vector<Mesh::Index>{0, 1, 2});
        }

        Triangle(const std::vector<Position> &positions, gl::Texture2d &texture, Colorf color = colors::White):
                Triangle(texture, color) {
            std::vector<Mesh::Vertex> vertices;
            vertices.reserve(3);
            for (std::size_t i = 0; i <= 3; ++i)
                vertices.emplace_back(positions[i]);
            this->mesh.set_data(vertices);
        }

        Triangle(Colorf color = colors::White):
            Triangle(Application::get_instance().get_resource_manager().get_white_texture(), color) { }
        Triangle(const std::vector<Position> &positions, Colorf color = colors::White):
            Triangle(positions, Application::get_instance().get_resource_manager().get_white_texture(), color) { }
        Triangle(const std::vector<Mesh::Vertex> &vertices, Colorf color = colors::White):
            Triangle(vertices,  Application::get_instance().get_resource_manager().get_white_texture(), color) { }

        void edit_point(std::size_t idx, const Position &position) {
            this->mesh.get_vertices()[idx].position = position;
        }

        void edit_point(std::size_t idx, const Mesh::Vertex &vertex) {
            this->mesh.get_vertices()[idx] = vertex;
        }

        virtual void on_draw(Renderer &renderer, float dt) override { }
};

} // namespace cmw::shapes
