#pragma once

#include "renderer.hpp"
#include "resource_manager.hpp"
#include "window.hpp"

namespace cmw {

class Application {
    public:
        Application(int window_w, int window_h, std::string window_name):
            window(window_w, window_h, window_name), renderer(resource_manager) { }
        ~Application() = default;

        inline Renderer        &get_renderer()         { return this->renderer; }
        inline ResourceManager &get_resource_manager() { return this->resource_manager; }
        inline Window          &get_window()           { return this->window; }

    private:
        Window window;
        ResourceManager resource_manager;
        Renderer renderer;
};

} // namespace cmw
