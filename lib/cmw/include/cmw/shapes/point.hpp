#pragma once

#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>

#include "../gl/texture.hpp"
#include "../color.hpp"
#include "../mesh.hpp"
#include "../platform.h"

namespace cmw::shapes {

class Point {
    public:
        Point(gl::Texture2d &texture, Colorf color = colors::White, GLfloat width = 1.0f): mesh(texture, color), width(width) { }

        Point(const glm::vec3 &position, gl::Texture2d &texture, Colorf color = colors::White, GLfloat width = 1.0f):
                Point(texture, color, width) {
            set_position(position);
        }

        void set_position(const glm::vec3 &position) {
            this->get_mesh().set_data({(Mesh::Vertex){position}});
        }

        void on_draw() {
            glPointSize(this->width);
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
