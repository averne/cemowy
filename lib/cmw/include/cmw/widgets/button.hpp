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

#include "cmw/shapes/rectangle.hpp"
#include "cmw/widgets/widget.hpp"

namespace cmw::widgets {

class Button: public Widget {
    public:
        Button(Widget *parent, const std::vector<Position> &positions, const std::string &text):
                Widget(parent), text(text), inner(positions), outer(positions) {

        }

        ~Button() = default;

        void draw(Renderer &renderer, float dt) override { CMW_TRACE("Button::draw\n"); }

        bool collides(const Position &position) const override {
            auto &vertices = this->inner.get_mesh().get_vertices();
            return vertices[0].position <= position && position <= vertices[1].position;
        }

        void on_draw(Renderer &renderer, float dt) override { }

        void on_hover(input::MouseMovedEvent &event) override {
            // this->inner.set_blend_color({1.0f, 0.0f, 0.0f, 1.0f});
        }

        void on_click(input::MouseButtonPressedEvent &event) override {
            // this->outer.set_blend_color({0.0f, 1.0f, 0.0f, 1.0f});
        }

    protected:
        std::string text;
        shapes::Rectangle inner, outer;
};

} // namespace cmw::widgets
