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

#include <utility>

#include "cmw/core/mesh.hpp"
#include "cmw/utils/color.hpp"

namespace cmw {

class Renderer;

namespace shapes {

class Shape {
    public:
        template <typename ...Args>
        Shape(Args &&...args): mesh(std::forward<Args>(args)...) { }

        virtual ~Shape() = default;

        virtual void on_draw(Renderer &renderer, float dt) = 0;

        inline Mesh &get_mesh() { return this->mesh; }
        inline const Mesh &get_mesh() const { return this->mesh; }

        inline Colorf get_blend_color() const { return this->mesh.get_blend_color(); }
        inline void set_blend_color(Colorf color) { this->get_mesh().get_blend_color() = color; }

    protected:
        Mesh mesh;
};

} // namespace shapes
} // namespace cmw
