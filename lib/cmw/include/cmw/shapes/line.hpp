#pragma once

#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>

#include "../gl/texture.hpp"
#include "../color.hpp"
#include "../mesh.hpp"
#include "../platform.h"

namespace cmw::shapes {

class Line {
    public:
        Line(gl::Texture2d &texture, Colorf color = colors::White, GLfloat width = 1.0f): mesh(texture, color), width(width) { }

        Line(const std::vector<glm::vec3> &positions, gl::Texture2d &texture, Colorf color = colors::White, GLfloat width = 1.0f):
                Line(texture, color, width) {
            Mesh::Index idx = 1;
            std::vector<Mesh::Vertex> vertices;
            std::vector<Mesh::Index> indices;

            vertices.reserve(positions.size());
            for (auto &pos: positions)
                vertices.emplace_back(pos);

            indices.reserve(2 * positions.size() - 2);
            indices.emplace_back(idx - 1);
            for (; idx < vertices.size() - 2; ++idx)
                indices.emplace_back(idx), indices.emplace_back(idx);
            indices.emplace_back(idx);

            this->get_mesh().set_data(vertices, indices);
        }

        template <typename ...Args>
        void add_points(Args &&...positions) {
            auto &vertices = this->mesh.get_vertices();
            auto &indices  = this->mesh.get_indices();
            Mesh::Index idx = vertices.size();

            vertices.reserve(sizeof...(Args));
            (vertices.emplace_back(positions), ...);

            indices.reserve(2 * sizeof...(Args));
            indices.emplace_back(idx - 1);
            for (; idx < vertices.size() - 1; ++idx)
                indices.emplace_back(idx), indices.emplace_back(idx);
            indices.emplace_back(idx);

            this->get_mesh().fill_buffers();
        }

        void edit_point(std::size_t idx, const glm::vec3 &position) {
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

} // namespace cmw::shapes