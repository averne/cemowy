#pragma once

#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>

#include "shape.hpp"
#include "../gl/texture.hpp"
#include "../color.hpp"
#include "../mesh.hpp"
#include "../platform.h"

namespace cmw::shapes {

class Point: public Shape {
    public:
        Point(gl::Texture2d &texture, Colorf color = colors::White, GLfloat width = 1.0f): Shape(texture, color), width(width) { }

        Point(const glm::vec3 &position, gl::Texture2d &texture, Colorf color = colors::White, GLfloat width = 1.0f):
                Point(texture, color, width) {
            set_position(position);
        }

        void set_position(const glm::vec3 &position) {
            this->get_mesh().set_data({(Mesh::Vertex){position}});
            this->get_mesh().fill_buffers();
        }

        virtual void on_draw() override {
            glPointSize(this->width);
        }

        inline GLfloat get_width() const { return this->width; }
        inline void set_width(GLfloat width) { this->width = width; }

    private:
        GLfloat width;
};

} // namespace cmw::shapes
