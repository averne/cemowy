#pragma once

#include <vector>
#include <glm/glm.hpp>

#include "../gl/texture.hpp"
#include "../color.hpp"
#include "../mesh.hpp"
#include "../platform.h"

namespace cmw::shapes {

class Circle {
    public:
        Circle(gl::Texture2d &texture, Colorf color = colors::White): mesh(texture, color) { }

        Circle(const glm::vec3 &position, float radius, gl::Texture2d &texture, Colorf color = colors::White, std::size_t segments = 24):
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

        void set_position(const glm::vec3 &position) {
            auto &vertices = this->get_mesh().get_vertices();
            glm::vec3 delta = position - vertices[0].position;
            for (auto &vertex: vertices)
                vertex.position += delta;
            this->get_mesh().fill_buffers();
        }

        void set_radius(float radius) {
            auto &vertices = this->get_mesh().get_vertices();
            glm::vec3 &center = vertices[0].position;
            float ratio = radius / glm::distance(vertices[0].position, vertices[1].position);
            for (auto &vertex: vertices) {
                vertex.position.x -= (vertex.position.x - center.x) * (1.0f - ratio);
                vertex.position.y -= (vertex.position.y - center.y) * (1.0f - ratio);
            }
            this->get_mesh().fill_buffers();
        }

        void on_draw() { };

        inline Mesh &get_mesh() { return this->mesh; }

        inline Colorf get_color() const { return this->mesh.get_blend_color(); }
        inline void set_color(Colorf color) { this->get_mesh().get_blend_color() = color; }

    private:
        Mesh mesh;
};

} // namespace cmw::shapes
