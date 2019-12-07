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

#include <vector>

#include "cmw/core/input.hpp"
#include "cmw/utils/position.hpp"

namespace cmw {

class Renderer;

namespace widgets {

class Widget {
    public:
        Widget(Widget *parent): parent(parent) {
            if (parent)
                parent->add_child(this);
        }

        virtual ~Widget() = default;

        inline void add_child(Widget *child) { this->children.push_back(child); }

        virtual void draw(Renderer &renderer, const glm::mat4 &model, float dt) = 0;

        virtual bool collides(const Position &position) const = 0;

        virtual void on_draw(Renderer &renderer, float dt) = 0;
        virtual void on_hover(input::MouseMovedEvent &event) = 0;
        virtual void on_click(input::MouseButtonPressedEvent &event) = 0;

    protected:
        Widget *parent;
        std::vector<Widget *> children;
};

} // namespace widgets
} // namespace cmw
