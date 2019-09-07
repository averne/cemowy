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

#include "platform.h"

namespace cmw {

class PerspectiveCamera {
    public:
        enum class Movement: uint8_t {
            Forward,
            Backward,
            Left,
            Right,
        };

        PerspectiveCamera(const glm::vec3 &pos = glm::vec3(0.0f, 0.0f, 0.0f), const glm::vec3 &front = glm::vec3(0.0f, 0.0f, -1.0f),
                GLfloat fov = 45.0f, float speed = 0.2f, float sensitivity = 0.05f):
                pos(pos), front(front), fov(fov), speed(speed), sensitivity(sensitivity) {
            glm::vec3 n_front = glm::normalize(this->front);
            this->yaw = glm::degrees(atan2(n_front.x, n_front.z));
            this->pitch = glm::degrees(asin(-n_front.y));
            update();
        }

        PerspectiveCamera(GLfloat pos_x, GLfloat pos_y, GLfloat pos_z,
                GLfloat front_x, GLfloat front_y, GLfloat front_z, GLfloat fov = 45.0f,
                float speed = 0.2f, float sensitivity = 0.05f):
                PerspectiveCamera({pos_x, pos_y, pos_z}, {front_x, front_y, front_z}, fov, speed, sensitivity) { }

        void move(Movement direction) {
            switch (direction) {
                case Movement::Forward:
                    this->pos += this->speed * this->front;
                    break;
                case Movement::Backward:
                    this->pos -= this->speed * this->front;
                    break;
                case Movement::Left:
                    this->pos -= glm::normalize(glm::cross(this->front, glm::vec3(0.0f, 1.0f, 0.0f))) * this->speed;
                    break;
                case Movement::Right:
                    this->pos += glm::normalize(glm::cross(this->front, glm::vec3(0.0f, 1.0f, 0.0f))) * this->speed;
                    break;
            }
            this->pos.y = 0.0f;
            update();
        }

        void rotate(float x, float y) {
            if (std::isnan(this->mouse_x) && std::isnan(this->mouse_y))
                this->mouse_x = x, this->mouse_y = y;

            this->yaw   -= (x - this->mouse_x) * this->sensitivity;
            this->pitch += (this->mouse_y - y) * this->sensitivity;
            this->pitch  = std::clamp(this->pitch, -89.0f, 89.0f);

            this->mouse_x = x, this->mouse_y = y;

            this->front = glm::normalize(glm::vec3(
                std::sin(glm::radians(this->yaw)) * std::cos(glm::radians(this->pitch)),
                std::sin(glm::radians(this->pitch)),
                std::cos(glm::radians(this->yaw)) * std::cos(glm::radians(this->pitch))
            ));

            update();
        }

        void zoom(float z) {
            this->fov -= z;
            this->fov = std::clamp(this->fov, 1.0f, 45.0f);
            update();
        }

        inline const glm::mat4 &get_proj()      const { return this->proj; }
        inline const glm::mat4 &get_view()      const { return this->view; }
        inline const glm::mat4 &get_view_proj() const { return this->view_proj; }

        inline const glm::vec3 &get_pos()   const { return this->pos; }
        inline const glm::vec3 &get_front() const { return this->front; }

        void update() {
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
                const glm::vec3 &position = {0.0f, 0.0f, 0.0f}, float rotation = 0.0f):
                position(position), rotation(rotation), proj(glm::ortho(left, right, bottom, top, near, far)) {
            this->offset = {glm::abs(left - right) / 2.0f, glm::abs(top - bottom) / 2.0f, 0.0f};
            update();
        }

        ~OrthographicCamera() = default;

        inline void move(const glm::vec3 &dposition) { this->position += dposition; update(); }
        inline void rotate(float drotation) { this->rotation += drotation; update(); }

        inline const glm::vec3 &get_position() const { return this->position; }
        inline void set_position(const glm::vec3 &position) { this->position = position; update(); }

        inline float get_rotation() const { return this->rotation; }
        inline void set_rotation(float rotation) { this->rotation = rotation; update(); }

        inline const glm::mat4 &get_view()      const { return this->view; }
        inline const glm::mat4 &get_proj()      const { return this->proj; }
        inline const glm::mat4 &get_view_proj() const { return this->view_proj; }

        void update() {
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
