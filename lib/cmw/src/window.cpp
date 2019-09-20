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

#include <string>
#include <stdexcept>
#include <utility>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "cmw/core/window.hpp"

namespace cmw {

Window::Window(int w, int h, const std::string &name, int x, int y, GLboolean resizable, GlVersion ver) {
    CMW_TRACE("Creating window object\n");
    glfwInit();
    set_gl_version(ver);
    hint(std::pair{GLFW_RESIZABLE, resizable});

    CMW_TRY_THROW((this->window = glfwCreateWindow(w, h, name.c_str(), nullptr, nullptr)),
        std::runtime_error("Could not create window object"));
    make_ctx_current();
    set_pos(x, y);
    glfwSetWindowUserPointer(get_window(), this);

    CMW_TRY_THROW(gladLoadGLLoader((GLADloadproc)glfwGetProcAddress),
        std::runtime_error("Failed to initialize glad"));

    this->input_manager.set_window(get_window());
}

} // namespace cmw
