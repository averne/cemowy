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

#include "cmw/core/camera.hpp"

namespace cmw {


PerspectiveCamera::PerspectiveCamera(const Position &pos, const Position &front, GLfloat fov, float speed, float sensitivity):
        pos(pos), front(front), fov(fov), speed(speed), sensitivity(sensitivity) {
    Position n_front = glm::normalize(this->front);
    this->yaw = glm::degrees(atan2(n_front.x, n_front.z));
    this->pitch = glm::degrees(asin(-n_front.y));
    update();
}

PerspectiveCamera::PerspectiveCamera(GLfloat pos_x, GLfloat pos_y, GLfloat pos_z,
        GLfloat front_x, GLfloat front_y, GLfloat front_z, GLfloat fov, float speed, float sensitivity):
        PerspectiveCamera({pos_x, pos_y, pos_z}, {front_x, front_y, front_z}, fov, speed, sensitivity) { }

void PerspectiveCamera::move(Movement direction) {
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

void PerspectiveCamera::rotate(float x, float y) {
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

void PerspectiveCamera::zoom(float z) {
    this->fov -= z;
    this->fov = std::clamp(this->fov, 1.0f, 45.0f);
    update();
}


OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top, float near , float far,
        const Position &position, float rotation):
        position(position), rotation(rotation), proj(glm::ortho(left, right, bottom, top, near, far)) {
    this->offset = {glm::abs(left - right) / 2.0f, glm::abs(top - bottom) / 2.0f, 0.0f};
    update();
}

} // namespace cmw
