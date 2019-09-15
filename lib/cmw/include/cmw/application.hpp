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

#include "imgui.hpp"
#include "log.hpp"
#include "renderer.hpp"
#include "resource_manager.hpp"
#include "window.hpp"

namespace cmw {

class Application {
    CMW_NON_COPYABLE(Application);
    CMW_NON_MOVEABLE(Application);

    public:
        template <typename ...Args>
        Application(Args &&...args): window(std::forward<Args>(args)...), renderer(resource_manager) {
            this->instance = this;
            cmw::log::initialize();
            cmw::imgui::initialize(&this->window);
        }

        ~Application() {
            cmw::imgui::finalize();
            cmw::log::finalize();
        }

        static Application &get_instance() { return *instance; }

        template <typename T>
        inline T get_time() const {
            return (T)glfwGetTime();
        }

        inline Renderer        &get_renderer()         { return this->renderer; }
        inline ResourceManager &get_resource_manager() { return this->resource_manager; }
        inline Window          &get_window()           { return this->window; }

    protected:
        static inline Application *instance;

        Window window;
        ResourceManager resource_manager;
        Renderer renderer;
};

} // namespace cmw
