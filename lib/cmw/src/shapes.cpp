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

#include "cmw/core/application.hpp"

#include "cmw/shapes.hpp"

namespace cmw::shapes {

Circle::Circle(gl::Texture2d &texture, Colorf color): Shape(texture, color) { }

Circle::Circle(const std::vector<Mesh::Vertex> &vertices, const std::vector<Mesh::Index> &indices,
        gl::Texture2d &texture, Colorf color):
        Circle(texture, color) {
    this->mesh.set_data(vertices, indices);
}

Circle::Circle(const Position &position, float radius, gl::Texture2d &texture, Colorf color, std::size_t segments):
        Circle(texture, color) {
    std::vector<Mesh::Vertex> vertices;
    std::vector<Mesh::Index> indices;
    Mesh::Index idx = 1;

    vertices.reserve(segments + 1);
    indices.reserve(3 * segments);

    vertices.push_back(position);
    for (float angle = 0; angle < glm::two_pi<float>(); angle += glm::two_pi<float>() / segments, ++idx) {
        float x = glm::cos(angle), y = glm::sin(angle);
        vertices.push_back({{position.x + x * radius, position.y + y * radius, position.z}, {(x + 1) / 2, (y + 1) / 2}});
        indices.insert(indices.end(), {0, idx, idx + 1});
    }
    indices.back() = 1; // Set last point to beginning of circle

    this->mesh.set_data(vertices, indices);
}

Line::Line(gl::Texture2d &texture, Colorf color, GLfloat width): Shape(texture, color), width(width) { }

Line::Line(const std::vector<Position> &positions, gl::Texture2d &texture, Colorf color, GLfloat width):
        Line(texture, color, width) {
    Mesh::Index idx = 1;
    std::vector<Mesh::Vertex> vertices;
    std::vector<Mesh::Index> indices;

    vertices.reserve(positions.size());
    for (const auto &pos: positions)
        vertices.emplace_back(pos);

    indices.reserve(2 * positions.size() - 2);
    indices.emplace_back(idx - 1);
    for (; idx < vertices.size() - 2; ++idx)
        indices.emplace_back(idx), indices.emplace_back(idx);
    indices.emplace_back(idx);

    this->get_mesh().set_data(vertices, indices);
}

Point::Point(gl::Texture2d &texture, Colorf color, GLfloat width): Shape(texture, color), width(width) {
    this->get_mesh().set_data(std::vector<Mesh::Index>{0});
}

Point::Point(const Position &position, gl::Texture2d &texture, Colorf color, GLfloat width):
        Point(texture, color, width) {
    set_position(position);
}

Rectangle::Rectangle(gl::Texture2d &texture, Colorf color): Shape(texture, color) { }

Rectangle::Rectangle(const std::vector<Position> &positions, gl::Texture2d &texture, Colorf color):
        Rectangle(texture, color) {
    std::vector<Mesh::Vertex> vertices;
    vertices.reserve(4);
    for (std::size_t i = 0; i <= 4; ++i)
        vertices.emplace_back(positions[i]);
    this->mesh.set_data(vertices, {0, 1, 2, 2, 3, 0});
}

Rectangle::Rectangle(const std::vector<Mesh::Vertex> &vertices, gl::Texture2d &texture, Colorf color):
        Rectangle(texture, color) {
    this->mesh.set_data(vertices, {0, 1, 2, 2, 3, 0});
}

Triangle::Triangle(gl::Texture2d &texture, Colorf color): Shape(texture, color) {
    this->mesh.set_data(std::vector<Mesh::Index>{0, 1, 2});
}

Triangle::Triangle(const std::vector<Mesh::Vertex> &vertices, gl::Texture2d &texture, Colorf color):
        Shape(vertices, texture, color) {
    this->mesh.set_data(std::vector<Mesh::Index>{0, 1, 2});
}

Triangle::Triangle(const std::vector<Position> &positions, gl::Texture2d &texture, Colorf color):
        Triangle(texture, color) {
    std::vector<Mesh::Vertex> vertices;
    vertices.reserve(3);
    for (std::size_t i = 0; i <= 3; ++i)
        vertices.emplace_back(positions[i]);
    this->mesh.set_data(vertices);
}

} //namespace cmw::shapes
