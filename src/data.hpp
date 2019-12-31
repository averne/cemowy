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

#include <glm/glm.hpp>

struct Vertex {
    glm::vec3 coords;
    glm::vec2 tex_coords;
};

constexpr Vertex vertices[] = {
    { glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec2(0.0f, 0.0f) },
    { glm::vec3( 0.5f, -0.5f, -0.5f), glm::vec2(1.0f, 0.0f) },
    { glm::vec3( 0.5f,  0.5f, -0.5f), glm::vec2(1.0f, 1.0f) },
    { glm::vec3( 0.5f,  0.5f, -0.5f), glm::vec2(1.0f, 1.0f) },
    { glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec2(0.0f, 1.0f) },
    { glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec2(0.0f, 0.0f) },

    { glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec2(0.0f, 0.0f) },
    { glm::vec3( 0.5f, -0.5f,  0.5f), glm::vec2(1.0f, 0.0f) },
    { glm::vec3( 0.5f,  0.5f,  0.5f), glm::vec2(1.0f, 1.0f) },
    { glm::vec3( 0.5f,  0.5f,  0.5f), glm::vec2(1.0f, 1.0f) },
    { glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec2(0.0f, 1.0f) },
    { glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec2(0.0f, 0.0f) },

    { glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec2(1.0f, 0.0f) },
    { glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec2(1.0f, 1.0f) },
    { glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec2(0.0f, 1.0f) },
    { glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec2(0.0f, 1.0f) },
    { glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec2(0.0f, 0.0f) },
    { glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec2(1.0f, 0.0f) },

    { glm::vec3( 0.5f,  0.5f,  0.5f), glm::vec2(1.0f, 0.0f) },
    { glm::vec3( 0.5f,  0.5f, -0.5f), glm::vec2(1.0f, 1.0f) },
    { glm::vec3( 0.5f, -0.5f, -0.5f), glm::vec2(0.0f, 1.0f) },
    { glm::vec3( 0.5f, -0.5f, -0.5f), glm::vec2(0.0f, 1.0f) },
    { glm::vec3( 0.5f, -0.5f,  0.5f), glm::vec2(0.0f, 0.0f) },
    { glm::vec3( 0.5f,  0.5f,  0.5f), glm::vec2(1.0f, 0.0f) },

    { glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec2(0.0f, 1.0f) },
    { glm::vec3( 0.5f, -0.5f, -0.5f), glm::vec2(1.0f, 1.0f) },
    { glm::vec3( 0.5f, -0.5f,  0.5f), glm::vec2(1.0f, 0.0f) },
    { glm::vec3( 0.5f, -0.5f,  0.5f), glm::vec2(1.0f, 0.0f) },
    { glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec2(0.0f, 0.0f) },
    { glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec2(0.0f, 1.0f) },

    { glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec2(0.0f, 1.0f) },
    { glm::vec3( 0.5f,  0.5f, -0.5f), glm::vec2(1.0f, 1.0f) },
    { glm::vec3( 0.5f,  0.5f,  0.5f), glm::vec2(1.0f, 0.0f) },
    { glm::vec3( 0.5f,  0.5f,  0.5f), glm::vec2(1.0f, 0.0f) },
    { glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec2(0.0f, 0.0f) },
    { glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec2(0.0f, 1.0f) },
};

struct CubeParam {
    glm::vec3 pos, rot_axis;
};

constexpr CubeParam cube_params[] = {
    { glm::vec3( 0.0f,  0.0f,   0.0f), glm::vec3(-0.85f, -0.13f, -0.44f) },
    { glm::vec3( 2.0f,  5.0f, -15.0f), glm::vec3(-0.13f, -0.97f, -0.93f) },
    { glm::vec3(-1.5f, -2.2f, - 2.5f), glm::vec3( 0.70f,  0.30f,  0.06f) },
    { glm::vec3(-3.8f, -2.0f, -12.3f), glm::vec3(-0.87f,  0.37f,  0.51f) },
    { glm::vec3( 2.4f, -0.4f, - 3.5f), glm::vec3( 0.46f,  0.99f,  0.27f) },
    { glm::vec3(-1.7f,  3.0f, - 7.5f), glm::vec3(-0.85f,  0.87f, -0.37f) },
    { glm::vec3( 1.3f, -2.0f, - 2.5f), glm::vec3(-0.62f,  0.38f,  0.84f) },
    { glm::vec3( 1.5f,  2.0f, - 2.5f), glm::vec3(-0.29f,  0.51f,  0.76f) },
    { glm::vec3( 1.5f,  0.2f, - 1.5f), glm::vec3(-0.95f,  0.77f,  0.55f) },
    { glm::vec3(-1.3f,  1.0f, - 1.5f), glm::vec3( 0.93f, -0.13f,  0.85f) },
};
