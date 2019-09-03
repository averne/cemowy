#pragma once

#include <vector>
#include <glm/glm.hpp>

#include "../color.hpp"
#include "../mesh.hpp"
#include "../platform.h"

namespace cmw::elements {

class Line {
    public:
        Line(GLfloat width = 1.0f, Colorf color = colors::White): width(width), color(color) { }
        Line(std::vector<glm::vec3> positions, GLfloat width = 1.0f, Colorf color = colors::White): Line(width, color) {
            this->vertices.reserve(positions.size());
            for (auto &pos: positions)
                this->vertices.push_back({pos, glm::vec2(0.5f, 0.5f)});
            this->mesh.fill_buffers(this->vertices);
        }

        ~Line() = default;

        template <typename ...Args>
        void add_points(Args &&...positions) {
            (this->vertices.push_back({positions, glm::vec2(0.5f, 0.5f)}), ...);
            this->mesh.fill_buffers(this->vertices);
        }

        void edit_point(std::size_t idx, glm::vec3 position) {
            this->vertices[idx].position = position;
            this->mesh.fill_buffers(this->vertices);
        }

        void on_draw() {
            glLineWidth(this->width);
        };

        inline const std::vector<Mesh::Vertex> &get_vertices() const { return this->vertices; }
        inline const Mesh &get_mesh() const { return this->mesh; }

        inline Colorf get_color() const { return this->color; }
        inline void set_color(Colorf color) { this->color = color; }
        inline GLfloat get_width() const { return this->width; }
        inline void set_width(GLfloat width) { this->width = width; }

    private:
        GLfloat width;
        Colorf color;
        std::vector<Mesh::Vertex> vertices;
        Mesh mesh;
};

} // namespace cmw::elements
