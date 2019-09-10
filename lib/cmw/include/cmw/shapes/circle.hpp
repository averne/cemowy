#pragma once

#include <vector>
#include <glm/glm.hpp>

#include "shape.hpp"
#include "../gl/texture.hpp"
#include "../color.hpp"
#include "../mesh.hpp"
#include "../position.hpp"
#include "../platform.h"

namespace cmw::shapes {

class Circle: public Shape {
    public:
        Circle(gl::Texture2d &texture, Colorf color = colors::White): Shape(texture, color) { }

        Circle(const Position &position, float radius, gl::Texture2d &texture, Colorf color = colors::White, std::size_t segments = 24):
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

        void set_position(const Position &position) {
            auto &vertices = this->get_mesh().get_vertices();
            Position delta = position - vertices[0].position;
            for (auto &vertex: vertices)
                vertex.position += delta;
            this->get_mesh().fill_buffers();
        }

        void set_radius(float radius) {
            auto &vertices = this->get_mesh().get_vertices();
            Position &center = vertices[0].position;
            float ratio = radius / glm::distance((glm::vec3)vertices[0].position, (glm::vec3)vertices[1].position);
            for (auto &vertex: vertices) {
                vertex.position.x -= (vertex.position.x - center.x) * (1.0f - ratio);
                vertex.position.y -= (vertex.position.y - center.y) * (1.0f - ratio);
            }
            this->get_mesh().fill_buffers();
        }

        virtual void on_draw() override { }
};

} // namespace cmw::shapes
