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

#include <string>
#include <utility>
#include <memory>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "cmw/core/input.hpp"
#include "cmw/platform.h"

namespace cmw {

struct GlVersion {
    int maj, min, profile;
};

class Window {
    public:
        Window(int w, int h, const std::string &name, int x = 0, int y = 0, GLboolean resizable = GL_TRUE,
                GlVersion ver = {4, 3, GLFW_OPENGL_CORE_PROFILE});

        ~Window() {
            CMW_TRACE("Destructing window object\n");
            glfwDestroyWindow(get_window());
            glfwTerminate();
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
            this->input_manager.process_nx_events(get_window());
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

        inline       input::InputManager &get_input_manager()       { return this->input_manager; }
        inline const input::InputManager &get_input_manager() const { return this->input_manager; }

        template <typename T> std::size_t register_callback(input::InputManager::Callback<T> cb) {
            return get_input_manager().register_callback<T>(cb);
        }

        template <typename T> void remove_callback(std::size_t handle) {
            get_input_manager().remove_callback<T>(handle);
        }

    protected:
        GLFWwindow *window;
        input::InputManager input_manager;
};

} // namespace cmw
