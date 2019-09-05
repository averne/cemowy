#pragma once

#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>

#include "../gl/texture.hpp"
#include "../color.hpp"
#include "../mesh.hpp"
#include "../platform.h"

namespace cmw::elements {

class Line {
    public:
        Line(gl::Texture2d &texture, Colorf color = colors::White, GLfloat width = 1.0f): mesh(texture, color), width(width) { }

        Line(std::vector<glm::vec3> positions, gl::Texture2d &texture, Colorf color = colors::White, GLfloat width = 1.0f):
                Line(texture, color, width) {
            std::vector<Mesh::Vertex> vertices;
            vertices.reserve(positions.size());
            for (auto &pos: positions)
                vertices.emplace_back(pos);
            this->get_mesh().set_vertices(vertices);
        }

        ~Line() = default;

        template <typename ...Args>
        void add_points(Args &&...positions) {
            auto &vertices = this->mesh.get_vertices();
            vertices.reserve(sizeof...(Args));
            (vertices.emplace_back(positions), ...);
            this->get_mesh().fill_buffers();
        }

        void edit_point(std::size_t idx, glm::vec3 position) {
            this->get_mesh().get_vertices()[idx].position = position;
            this->get_mesh().fill_buffers();
        }

        void on_draw() {
            glLineWidth(this->width);
        };

        inline Mesh &get_mesh() { return this->mesh; }

        inline Colorf get_color() const { return this->mesh.get_blend_color(); }
        inline void set_color(Colorf color) { this->get_mesh().get_blend_color() = color; }

        inline GLfloat get_width() const { return this->width; }
        inline void set_width(GLfloat width) { this->width = width; }

    private:
        Mesh mesh;
        GLfloat width;
};

} // namespace cmw::elements
