#pragma once

#include "imgui.hpp"
#include "log.hpp"
#include "renderer.hpp"
#include "resource_manager.hpp"
#include "window.hpp"

namespace cmw {

class Application {
    public:
        template <typename ...Args>
        Application(Args &&...args): window(std::forward<Args>(args)...), renderer(resource_manager) {
            cmw::log::initialize();
            cmw::imgui::initialize(&this->window);
        }

        ~Application() {
            cmw::imgui::finalize();
            cmw::log::finalize();
        };

        template <typename T>
        inline T get_time() const {
            return (T)glfwGetTime();
        }

        inline Renderer        &get_renderer()         { return this->renderer; }
        inline ResourceManager &get_resource_manager() { return this->resource_manager; }
        inline Window          &get_window()           { return this->window; }

    protected:
        Window window;
        ResourceManager resource_manager;
        Renderer renderer;
};

} // namespace cmw
