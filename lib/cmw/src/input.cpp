#include <cstdint>
#include <limits>
#include <utility>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <cmw/window.hpp>
#include <cmw/platform.h>

#include <cmw/input.hpp>

namespace cmw {

void InputManager::set_window(GLFWwindow *window) {
#ifndef CMW_SWITCH
    glfwSetKeyCallback(window, keys_cb);
    glfwSetCharCallback(window, char_cb);
    glfwSetCursorPosCallback(window, cursor_cb);
    glfwSetScrollCallback(window, scroll_cb);
    glfwSetMouseButtonCallback(window, click_cb);
    glfwSetWindowPosCallback(window, window_pos_cb);
    glfwSetWindowSizeCallback(window, window_size_cb);
    glfwSetWindowFocusCallback(window, window_focus_cb);
    glfwSetWindowCloseCallback(window, window_close_cb);
#endif
}

#ifdef CMW_SWITCH
void InputManager::process_nx_events(GLFWwindow *window) const {
    static std::array<float, CMW_SWITCH_KEY_LAST - CMW_SWITCH_KEY_FIRST + 1> nx_keys_time; // The rest are supported by glfw
    float time = glfwGetTime();
    u64 keys_down = hidKeysDown(CONTROLLER_P1_AUTO) | hidKeysHeld(CONTROLLER_P1_AUTO); // glfw internally calls hidScanInput

    // Buttons
    for (uint32_t key = CMW_SWITCH_KEY_FIRST; key <= CMW_SWITCH_KEY_LAST; ++key) {
        const uint32_t idx       = key - CMW_SWITCH_KEY_FIRST;
        const uint32_t libnx_key = CMW_BIT(idx);

        if ((keys_down & libnx_key) && (nx_keys_time[idx] == 0.0f)) {
            this->process(KeyPressedEvent(key, 0));       // Key pressed for the first time
            nx_keys_time[idx] = time;
        } else if ((keys_down & libnx_key) && (time - nx_keys_time[idx] >= key_held_threshold)) {
            this->process(KeyHeldEvent(key, 0));          // Key held for over min_held_time seconds
        } else if (!(keys_down & libnx_key) && (nx_keys_time[idx] > 0.0f)) {
            this->process(KeyReleasedEvent(key, 0));      // Key released
            nx_keys_time[idx] = 0.0f;
        }
    }

    // Touchscreen
    static bool had_touch_last_frame;
    static touchPosition pos;
    if (keys_down & KEY_TOUCH) {
        hidTouchRead(&pos, 0); // TODO: support multiple touch points
        if (!had_touch_last_frame)
            this->process(ScreenPressedEvent(pos.px, pos.py, pos.dx, pos.dy, pos.angle));
        else
            this->process(ScreenTouchedEvent(pos.px, pos.py, pos.dx, pos.dy, pos.angle));
        had_touch_last_frame = true;
    } else {
        if (had_touch_last_frame)
            this->process(ScreenReleasedEvent(pos.px, pos.py, pos.dx, pos.dy, pos.angle));
        had_touch_last_frame = false;
    }

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
    InputManager &tmp_this = ((Window *)glfwGetWindowUserPointer(window))->get_input_manager();
    tmp_this.process(MouseMovedEvent(x, y));
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

} // namespace cmw
