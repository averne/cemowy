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

#include "cmw/core/renderer.hpp"
#include "cmw/utils/position.hpp"
#include "cmw/widgets/widget.hpp"
#include "cmw/platform.h"

namespace cmw::widgets {

class Scene: public Widget {
    public:
        Scene(Position min, Position max): Widget(nullptr), min(min), max(max) {
            auto &im = Application::get_instance().get_window().get_input_manager();
#ifdef CMW_SWITCH
            im.register_callback<input::MouseMovedEvent>([this](auto &e) { this->on_hover(e); });
            im.register_callback<input::MouseButtonPressedEvent>([this](auto &e) { this->on_click(e); });
#else
            im.register_callback<input::MouseMovedEvent>([this](auto &e) { this->on_hover(e); });
            im.register_callback<input::MouseButtonPressedEvent>([this](auto &e) { this->on_click(e); });
#endif
        }

        ~Scene() {
            auto &im = Application::get_instance().get_window().get_input_manager();
            im.remove_callback<input::MouseMovedEvent>(this->pos_cb);
            im.remove_callback<input::MouseButtonPressedEvent>(this->click_cb);
        }

        void draw(Renderer &renderer, float dt) override {
            for (auto &child: this->children)
                child->draw(renderer, dt);
        }

        bool collides(const Position &position) const override {
            return this->min <= position && position <= this->max;
        }

        void on_draw(Renderer &renderer, float dt) override { }

        void on_hover(input::MouseMovedEvent &event) override {
            CMW_TRACE("x: %.3f, y: %.3f\n", event.get_x(), event.get_y());
            if (!collides({event.get_x(), event.get_y(), 0.0f})){
                this->hovered = false;
                return;
            }
            this->hovered = true;
            for (auto &child: this->children)
                child->on_hover(event);
        }

        void on_click(input::MouseButtonPressedEvent &event) override {
            if (!this->hovered)
                return;
            for (auto &child: this->children)
                child->on_click(event);
        }

    protected:
        bool hovered;
        Position min, max;
        std::size_t pos_cb, click_cb;
};


} // namespace cmw::widgets
