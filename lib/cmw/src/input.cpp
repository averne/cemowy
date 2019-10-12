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

#include <cstdint>
#include <limits>
#include <utility>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "cmw/core/application.hpp"
#include "cmw/core/input.hpp"
#include "cmw/core/window.hpp"
#include "cmw/utils/position.hpp"
#include "cmw/platform.h"

namespace cmw::input {

void InputManager::set_window(GLFWwindow *window) {
    glfwSetKeyCallback(window, keys_cb);
    glfwSetCursorPosCallback(window, cursor_cb);
    glfwSetMouseButtonCallback(window, click_cb);
    glfwSetCharCallback(window, char_cb);
    glfwSetScrollCallback(window, scroll_cb);
#ifdef CMW_PC
    glfwSetWindowPosCallback(window, window_pos_cb);
    glfwSetWindowSizeCallback(window, window_size_cb);
    glfwSetWindowFocusCallback(window, window_focus_cb);
    glfwSetWindowCloseCallback(window, window_close_cb);
#endif
}

#ifdef CMW_SWITCH
void InputManager::process_nx_events(GLFWwindow *window) const {
    // Joysticks
    constexpr float joystick_max = (float)std::numeric_limits<int16_t>::max();
    {
        JoystickPosition pos;
        hidJoystickRead(&pos, CONTROLLER_P1_AUTO, JOYSTICK_LEFT);
        if ((pos.dx + pos.dy) != 0)
            this->process(JoystickMovedEvent((float)pos.dx / joystick_max, (float)pos.dy / joystick_max, 1));
        hidJoystickRead(&pos, CONTROLLER_P1_AUTO, JOYSTICK_RIGHT);
        if ((pos.dx + pos.dy) != 0)
            this->process(JoystickMovedEvent((float)pos.dx / joystick_max, (float)pos.dy / joystick_max, 0));
    }
}
#endif

void InputManager::keys_cb(GLFWwindow *window, int key, int scancode, int action, int modifiers) {
    InputManager &tmp_this = ((Window *)glfwGetWindowUserPointer(window))->get_input_manager();
    if (action == GLFW_PRESS)
        tmp_this.process(KeyPressedEvent(key, modifiers));
    else if (action == GLFW_RELEASE)
        tmp_this.process(KeyReleasedEvent(key, modifiers));
    else
        tmp_this.process(KeyHeldEvent(key, modifiers));
}

void InputManager::char_cb(GLFWwindow *window, unsigned int codepoint) {
    InputManager &tmp_this = ((Window *)glfwGetWindowUserPointer(window))->get_input_manager();
    tmp_this.process(CharTypedEvent(codepoint));
}

void InputManager::cursor_cb(GLFWwindow *window, double x, double y) {
    auto [w, h] = Application::get_instance().get_window().get_size();
    InputManager &tmp_this = ((Window *)glfwGetWindowUserPointer(window))->get_input_manager();
    tmp_this.process(MouseMovedEvent(x, h - y));
}

void InputManager::scroll_cb(GLFWwindow *window, double x, double y) {
    InputManager &tmp_this = ((Window *)glfwGetWindowUserPointer(window))->get_input_manager();
    tmp_this.process(MouseScrolledEvent(x, y));
}

void InputManager::click_cb(GLFWwindow *window, int key, int action, int modifiers) {
    InputManager &tmp_this = ((Window *)glfwGetWindowUserPointer(window))->get_input_manager();
    if (action == GLFW_PRESS)
        tmp_this.process(MouseButtonPressedEvent(key, modifiers));
    else if (action == GLFW_RELEASE)
        tmp_this.process(MouseButtonReleasedEvent(key, modifiers));
    else
        tmp_this.process(MouseButtonHeldEvent(key, modifiers));
}

void InputManager::window_pos_cb(GLFWwindow* window, int x, int y) {
    InputManager &tmp_this = ((Window *)glfwGetWindowUserPointer(window))->get_input_manager();
    tmp_this.process(WindowMovedEvent(x, y));
}

void InputManager::window_size_cb(GLFWwindow* window, int width, int height) {
    InputManager &tmp_this = ((Window *)glfwGetWindowUserPointer(window))->get_input_manager();
    tmp_this.process(WindowResizedEvent(width, height));
}

void InputManager::window_focus_cb(GLFWwindow* window, int focused) {
    InputManager &tmp_this = ((Window *)glfwGetWindowUserPointer(window))->get_input_manager();
    if (focused)
        tmp_this.process(WindowFocusedEvent());
    else
        tmp_this.process(WindowDefocusedEvent());
}

void InputManager::window_close_cb(GLFWwindow* window) {
    InputManager &tmp_this = ((Window *)glfwGetWindowUserPointer(window))->get_input_manager();
    tmp_this.process(WindowClosedEvent());
}

} // namespace cmw::input
