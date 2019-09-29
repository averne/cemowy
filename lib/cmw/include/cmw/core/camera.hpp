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

#include <cstdint>
#include <cmath>
#include <algorithm>
#include <utility>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "cmw/utils/position.hpp"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wstrict-aliasing"

namespace cmw {

class PerspectiveCamera {
    public:
        enum class Movement: uint8_t {
            Forward,
            Backward,
            Left,
            Right,
        };

        PerspectiveCamera(const Position &pos = Position(0.0f, 0.0f, 0.0f), const Position &front = Position(0.0f, 0.0f, -1.0f),
                GLfloat fov = 45.0f, float speed = 0.2f, float sensitivity = 0.05f);

        PerspectiveCamera(GLfloat pos_x, GLfloat pos_y, GLfloat pos_z,
                GLfloat front_x, GLfloat front_y, GLfloat front_z, GLfloat fov = 45.0f,
                float speed = 0.2f, float sensitivity = 0.05f);

        void move(Movement direction);
        void rotate(float x, float y);
        void zoom(float z);

        inline const glm::mat4 &get_proj()      const { return this->proj; }
        inline const glm::mat4 &get_view()      const { return this->view; }
        inline const glm::mat4 &get_view_proj() const { return this->view_proj; }

        inline const Position &get_pos()   const { return *reinterpret_cast<const Position *>(&this->pos); }
        inline const Position &get_front() const { return *reinterpret_cast<const Position *>(&this->front); }

        inline void update() {
            this->proj = glm::perspective(glm::radians(this->fov), this->scr_w / this->scr_h, 0.1f, 100.0f);
            this->view = glm::lookAt(this->pos, this->pos + this->front, glm::vec3(0.0f, 1.0f, 0.0f));
            this->view_proj = this->proj * this->view;
        }

        inline void    set_speed(GLfloat speed)                    { this->speed = speed; }
        inline GLfloat get_speed() const                           { return this->speed; }
        inline void    set_sensitivity(GLfloat sensitivity)        { this->sensitivity = sensitivity; }
        inline GLfloat get_sensitivity() const                     { return this->sensitivity; }
        inline void    set_viewport_dims(std::pair<int, int> dims) { this->scr_w = dims.first, this->scr_h = dims.second; update(); }

    protected:
        glm::vec3 pos, front;
        GLfloat yaw, pitch, fov, speed, sensitivity;
        float scr_w = 1, scr_h = 1, mouse_x = NAN, mouse_y = NAN;;
        glm::mat4 view, proj, view_proj;
};

class OrthographicCamera {
    public:
        OrthographicCamera(float left, float right, float bottom, float top, float near , float far,
                const Position &position = {0.0f, 0.0f, 0.0f}, float rotation = 0.0f);

        inline void move(const Position &dposition) { this->position += (glm::vec3)dposition; update(); }
        inline void rotate(float drotation) { this->rotation += drotation; update(); }

        inline const Position &get_position() const { return *reinterpret_cast<const Position *>(&this->position); }
        inline void set_position(const Position &position) { this->position = position; update(); }

        inline float get_rotation() const { return this->rotation; }
        inline void set_rotation(float rotation) { this->rotation = rotation; update(); }

        inline const glm::mat4 &get_view()      const { return this->view; }
        inline const glm::mat4 &get_proj()      const { return this->proj; }
        inline const glm::mat4 &get_view_proj() const { return this->view_proj; }

        inline void update() {
            this->view = glm::translate(glm::mat4(1.0f), this->offset);
            this->view = glm::rotate(this->view, glm::radians(this->rotation), glm::vec3(0.0f, 0.0f, 1.0f));
            this->view = glm::translate(this->view, this->position - this->offset);

            this->view_proj = this->proj * this->view;
        }

    protected:
        glm::vec3 offset;
        glm::vec3 position = {0.0f, 0.0f, 0.0f};
        float rotation = 0.0f;

        glm::mat4 view, proj, view_proj;
};

} // namespace cmw

#pragma GCC diagnostic pop
