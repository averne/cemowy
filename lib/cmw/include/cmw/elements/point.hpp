#pragma once

#include <vector>
#include <glm/glm.hpp>

#include "../color.hpp"
#include "../mesh.hpp"
#include "../platform.h"

namespace cmw::elements {

class Point {
    public:
        Point(GLfloat width = 1.0f, Colorf color = colors::White): width(width), color(color) { }
        Point(glm::vec3 position, GLfloat width = 1.0f, Colorf color = colors::White): Point(width, color) {
            this->vertices.push_back({position, glm::vec2(0.5f, 0.5f)});
            this->mesh.fill_buffers(this->vertices);
        }

        ~Point() = default;

        void set_position(glm::vec3 position) {
            this->vertices[0].position = position;
            this->mesh.fill_buffers(this->vertices);
        }

        void on_draw() {
            glPointSize(this->width);
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
