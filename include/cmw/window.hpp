#pragma once

#include <initializer_list>
#include <algorithm>
#include <stdexcept>
#include <utility>
#include <memory>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "input.hpp"
#include "platform.h"

namespace cmw {

struct GlVersion {
    int maj, min, profile;
};

class Window {
    public:
        Window(int w, int h, const char *name, int x = 0, int y = 0, GLboolean resizable = GL_TRUE,
                GlVersion ver = {3, 3, GLFW_OPENGL_CORE_PROFILE}) {
            CMW_TRACE("Creating window object\n");
            set_gl_version(ver);
            hint(std::pair{GLFW_RESIZABLE, resizable});
            CMW_TRY_THROW((this->window = glfwCreateWindow(w, h, name, nullptr, nullptr)),
                std::runtime_error("Could not create window object"));
            make_ctx_current();
            set_pos(x, y);
            set_vsync(true);
            this->input_manager = std::make_unique<InputManager>(get_window());
        }

        ~Window() {
            CMW_TRACE("Destructing window object\n");
            glfwDestroyWindow(get_window());
        }

        void make_ctx_current() const {
            glfwMakeContextCurrent(get_window());
        }

        static void set_gl_version(GlVersion ver) {
            hint(std::pair{GLFW_CONTEXT_VERSION_MAJOR, ver.maj},
                std::pair{GLFW_CONTEXT_VERSION_MINOR, ver.min}, std::pair{GLFW_OPENGL_PROFILE, ver.profile});
        }

        template <typename ...Hints>
        static void hint(Hints &&...hints) {
            (glfwWindowHint(hints.first, hints.second), ...);
        }

        static void set_viewport(int w, int h, int x = 0, int y = 0) {
            glViewport(x, y, w, h);
        }

        static void poll_events() {
            glfwPollEvents();
        }

        void swap_buffers() const {
            glfwSwapBuffers(get_window());
        }

        void update() const {
            poll_events();
#ifdef CMW_SWITCH
            this->input_manager->process_nx_events(get_window());
#endif
            swap_buffers();
        }

        int get_key(int key) const {
            return glfwGetKey(get_window(), key);
        }

        static inline void set_vsync(bool val) { glfwSwapInterval(val); }

        inline void show()  const { glfwShowWindow(get_window()); }
        inline void hide()  const { glfwHideWindow(get_window()); }
        inline void focus() const { glfwFocusWindow(get_window()); }

        inline void set_should_close(bool val) const { glfwSetWindowShouldClose(get_window(), val); }
        inline bool get_should_close()         const { return glfwWindowShouldClose(get_window()); }

        inline void set_name(const char *name) const { glfwSetWindowTitle(get_window(), name); }
        inline void set_pos(int x, int y)      const { glfwSetWindowPos(get_window(), x, y); }
        inline std::pair<int, int> get_pos()   const { int x, y; glfwGetWindowPos(get_window(), &x, &y); return {x, y}; }
        inline void set_size(int w, int h)     const { glfwSetWindowSize(get_window(), w, h); }
        inline std::pair<int, int> get_size()  const { int w, h; glfwGetWindowSize(get_window(), &w, &h); return {w, h}; }

        inline GLFWwindow *get_window() const { return this->window; }
        inline InputManager *get_input_manager() const { return this->input_manager.get(); }

    protected:
        GLFWwindow *window;
        std::unique_ptr<InputManager> input_manager;
};

} // namespace cmw
