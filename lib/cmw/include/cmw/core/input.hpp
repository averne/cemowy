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

#include <cstdint>
#include <array>
#include <unordered_map>
#include <functional>
#include <utility>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "cmw/utils/area.hpp"
#include "cmw/utils/position.hpp"
#include "cmw/utils.hpp"
#include "cmw/platform.h"

namespace cmw::input {

// Same as GLFW's
enum MouseButtons: std::uint16_t {
    Button1              = GLFW_MOUSE_BUTTON_1,
    Button2              = GLFW_MOUSE_BUTTON_2,
    Button3              = GLFW_MOUSE_BUTTON_3,
    Button4              = GLFW_MOUSE_BUTTON_4,
    Button5              = GLFW_MOUSE_BUTTON_5,
    Button6              = GLFW_MOUSE_BUTTON_6,
    Button7              = GLFW_MOUSE_BUTTON_7,
    Button8              = GLFW_MOUSE_BUTTON_8,

    Left                 = Button1,
    Right                = Button2,
    Middle               = Button3,

    First                = Button1,
    Last                 = Button8,
};

enum Keys: std::uint16_t {
    KeyUnknown           = std::numeric_limits<std::uint16_t>::max(),

    KeySpace             = GLFW_KEY_SPACE,
    KeyApostrophe        = GLFW_KEY_APOSTROPHE,  /* ' */
    KeyComma             = GLFW_KEY_COMMA,  /* , */
    KeyMinus             = GLFW_KEY_MINUS,  /* - */
    KeyPeriod            = GLFW_KEY_PERIOD,  /* . */
    KeySlash             = GLFW_KEY_SLASH,  /* / */
    Key0                 = GLFW_KEY_0,
    Key1                 = GLFW_KEY_1,
    Key2                 = GLFW_KEY_2,
    Key3                 = GLFW_KEY_3,
    Key4                 = GLFW_KEY_4,
    Key5                 = GLFW_KEY_5,
    Key6                 = GLFW_KEY_6,
    Key7                 = GLFW_KEY_7,
    Key8                 = GLFW_KEY_8,
    Key9                 = GLFW_KEY_9,
    KeySemicolon         = GLFW_KEY_SEMICOLON,  /* ; */
    KeyEqual             = GLFW_KEY_EQUAL,  /* = */
    KeyA                 = GLFW_KEY_A,
    KeyB                 = GLFW_KEY_B,
    KeyC                 = GLFW_KEY_C,
    KeyD                 = GLFW_KEY_D,
    KeyE                 = GLFW_KEY_E,
    KeyF                 = GLFW_KEY_F,
    KeyG                 = GLFW_KEY_G,
    KeyH                 = GLFW_KEY_H,
    KeyI                 = GLFW_KEY_I,
    KeyJ                 = GLFW_KEY_J,
    KeyK                 = GLFW_KEY_K,
    KeyL                 = GLFW_KEY_L,
    KeyM                 = GLFW_KEY_M,
    KeyN                 = GLFW_KEY_N,
    KeyO                 = GLFW_KEY_O,
    KeyP                 = GLFW_KEY_P,
    KeyQ                 = GLFW_KEY_Q,
    KeyR                 = GLFW_KEY_R,
    KeyS                 = GLFW_KEY_S,
    KeyT                 = GLFW_KEY_T,
    KeyU                 = GLFW_KEY_U,
    KeyV                 = GLFW_KEY_V,
    KeyW                 = GLFW_KEY_W,
    KeyX                 = GLFW_KEY_X,
    KeyY                 = GLFW_KEY_Y,
    KeyZ                 = GLFW_KEY_Z,
    KeyLeftBracket       = GLFW_KEY_LEFT_BRACKET,  /* [ */
    KeyBackslash         = GLFW_KEY_BACKSLASH,  /* \ */
    KeyRightBracket      = GLFW_KEY_RIGHT_BRACKET,  /* ] */
    KeyGraveAccent       = GLFW_KEY_GRAVE_ACCENT,  /* ` */
    KeyWorld1            = GLFW_KEY_WORLD_1, /* non-US #1 */
    KeyWorld2            = GLFW_KEY_WORLD_2, /* non-US #2 */
    KeyEscape            = GLFW_KEY_ESCAPE,
    KeyEnter             = GLFW_KEY_ENTER,
    KeyTab               = GLFW_KEY_TAB,
    KeyBackspace         = GLFW_KEY_BACKSPACE,
    KeyInsert            = GLFW_KEY_INSERT,
    KeyDelete            = GLFW_KEY_DELETE,
    KeyRight             = GLFW_KEY_RIGHT,
    KeyLeft              = GLFW_KEY_LEFT,
    KeyDown              = GLFW_KEY_DOWN,
    KeyUp                = GLFW_KEY_UP,
    KeyPageUp            = GLFW_KEY_PAGE_UP,
    KeyPageDown          = GLFW_KEY_PAGE_DOWN,
    KeyHome              = GLFW_KEY_HOME,
    KeyEnd               = GLFW_KEY_END,
    KeyCapsLock          = GLFW_KEY_CAPS_LOCK,
    KeyScrollLock        = GLFW_KEY_SCROLL_LOCK,
    KeyNumLock           = GLFW_KEY_NUM_LOCK,
    KeyPrintScreen       = GLFW_KEY_PRINT_SCREEN,
    KeyPause             = GLFW_KEY_PAUSE,
    KeyF1                = GLFW_KEY_F1,
    KeyF2                = GLFW_KEY_F2,
    KeyF3                = GLFW_KEY_F3,
    KeyF4                = GLFW_KEY_F4,
    KeyF5                = GLFW_KEY_F5,
    KeyF6                = GLFW_KEY_F6,
    KeyF7                = GLFW_KEY_F7,
    KeyF8                = GLFW_KEY_F8,
    KeyF9                = GLFW_KEY_F9,
    KeyF10               = GLFW_KEY_F10,
    KeyF11               = GLFW_KEY_F11,
    KeyF12               = GLFW_KEY_F12,
    KeyF13               = GLFW_KEY_F13,
    KeyF14               = GLFW_KEY_F14,
    KeyF15               = GLFW_KEY_F15,
    KeyF16               = GLFW_KEY_F16,
    KeyF17               = GLFW_KEY_F17,
    KeyF18               = GLFW_KEY_F18,
    KeyF19               = GLFW_KEY_F19,
    KeyF20               = GLFW_KEY_F20,
    KeyF21               = GLFW_KEY_F21,
    KeyF22               = GLFW_KEY_F22,
    KeyF23               = GLFW_KEY_F23,
    KeyF24               = GLFW_KEY_F24,
    KeyF25               = GLFW_KEY_F25,
    KeyKp0               = GLFW_KEY_KP_0,
    KeyKp1               = GLFW_KEY_KP_1,
    KeyKp2               = GLFW_KEY_KP_2,
    KeyKp3               = GLFW_KEY_KP_3,
    KeyKp4               = GLFW_KEY_KP_4,
    KeyKp5               = GLFW_KEY_KP_5,
    KeyKp6               = GLFW_KEY_KP_6,
    KeyKp7               = GLFW_KEY_KP_7,
    KeyKp8               = GLFW_KEY_KP_8,
    KeyKp9               = GLFW_KEY_KP_9,
    KeyKpDecimal         = GLFW_KEY_KP_DECIMAL,
    KeyKpDivide          = GLFW_KEY_KP_DIVIDE,
    KeyKpMultiply        = GLFW_KEY_KP_MULTIPLY,
    KeyKpSubstract       = GLFW_KEY_KP_SUBTRACT,
    KeyKpAdd             = GLFW_KEY_KP_ADD,
    KeyKpEnter           = GLFW_KEY_KP_ENTER,
    KeyKpEqual           = GLFW_KEY_KP_EQUAL,
    KeyLeftShift         = GLFW_KEY_LEFT_SHIFT,
    KeyLeftControl       = GLFW_KEY_LEFT_CONTROL,
    KeyLeftAlt           = GLFW_KEY_LEFT_ALT,
    KeyLeftSuper         = GLFW_KEY_LEFT_SUPER,
    KeyRightShift        = GLFW_KEY_RIGHT_SHIFT,
    KeyRightControl      = GLFW_KEY_RIGHT_CONTROL,
    KeyRightAlt          = GLFW_KEY_RIGHT_ALT,
    KeyRightSuper        = GLFW_KEY_RIGHT_SUPER,
    KeyMenu              = GLFW_KEY_MENU,

    // Switch keys
    KeySwitchA           = GLFW_NX_KEY_A,
    KeySwitchB           = GLFW_NX_KEY_B,
    KeySwitchX           = GLFW_NX_KEY_X,
    KeySwitchY           = GLFW_NX_KEY_Y,
    KeySwitchLstick      = GLFW_NX_KEY_LSTICK,
    KeySwitchRstick      = GLFW_NX_KEY_RSTICK,
    KeySwitchL           = GLFW_NX_KEY_L,
    KeySwitchR           = GLFW_NX_KEY_R,
    KeySwitchZl          = GLFW_NX_KEY_ZL,
    KeySwitchZr          = GLFW_NX_KEY_ZR,
    KeySwitchPlus        = GLFW_NX_KEY_PLUS,
    KeySwitchMinus       = GLFW_NX_KEY_MINUS,
    KeySwitchDleft       = GLFW_NX_KEY_DLEFT,
    KeySwitchDup         = GLFW_NX_KEY_DUP,
    KeySwitchDright      = GLFW_NX_KEY_DRIGHT,
    KeySwitchDdown       = GLFW_NX_KEY_DDOWN,

    KeyFirst             = KeySpace,
    KeyLast              = KeySwitchDdown,
    KeySwitchFirst       = KeySwitchA,
    KeySwitchLast        = KeySwitchDdown,

    ModShift             = GLFW_MOD_SHIFT,
    ModControl           = GLFW_MOD_CONTROL,
    ModAlt               = GLFW_MOD_ALT,
    ModSuper             = GLFW_MOD_SUPER,
    ModCapsLock          = GLFW_MOD_CAPS_LOCK,
    ModNumLock           = GLFW_MOD_NUM_LOCK,
};

enum class EventType: uint8_t {
    Invalid,
    KeyPressed, KeyHeld, KeyReleased,
    CharTyped,
    MouseButtonPressed, MouseButtonHeld, MouseButtonReleased,
    MouseMoved,
    MouseScrolled,
    WindowResized, WindowMoved, WindowFocused, WindowDefocused, WindowClosed,
    ScreenPressed, ScreenTouched, ScreenReleased,
    JoystickMoved,
    Max,
};

#define DECL_EVENT_TYPE_GETTERS(type)                                                   \
    public:                                                                             \
        inline EventType get_type() const override { return EventType::type; }          \
        static inline constexpr EventType get_static_type() { return EventType::type; }

struct Event {
    virtual ~Event() = default;
    virtual inline EventType get_type() const { return EventType::Invalid; }
    static inline constexpr EventType get_static_type() { return EventType::Invalid; }
};

struct KeyEvent: public Event {
    inline KeyEvent(int key, int mods): key(key), mods(mods) { }

    inline int get_key()  const { return this->key; }
    inline int get_mods() const { return this->mods; }

    inline bool has_mod(int mod) const { return get_mods() & mod; }

    protected:
        int key, mods;
};

struct KeyPressedEvent: public KeyEvent {
    DECL_EVENT_TYPE_GETTERS(KeyPressed)

    inline KeyPressedEvent(int key, int mods): KeyEvent(key, mods) { }
};

struct KeyHeldEvent: public KeyEvent {
    DECL_EVENT_TYPE_GETTERS(KeyHeld)

    inline KeyHeldEvent(int key, int mods): KeyEvent(key, mods) {
        ++this->key_cnts[key];
    }

    inline int        get_repeats() const  { return key_cnts[get_key()]; }
    static inline int get_repeats(int key) { return key_cnts[key]; }

    static inline void reset_key(int key) { key_cnts[key] = 0; }

    protected:
        static inline std::array<std::uint32_t, Keys::KeyLast> key_cnts;
};

struct KeyReleasedEvent: public KeyEvent {
    DECL_EVENT_TYPE_GETTERS(KeyReleased)

    inline KeyReleasedEvent(int key, int mods): KeyEvent(key, mods) {
        KeyHeldEvent::reset_key(key);
    }
};

struct CharTypedEvent: public Event {
    DECL_EVENT_TYPE_GETTERS(CharTyped)

    inline CharTypedEvent(unsigned int codepoint): codepoint(codepoint) { }

    inline unsigned int get_codepoint() const { return this->codepoint; }

    protected:
        unsigned int codepoint;
};

struct MouseEvent: public Event {
    inline MouseEvent(float x, float y): pos(x, y) { }

    inline float get_x() const { return this->pos.x; }
    inline float get_y() const { return this->pos.y; }
    inline const Position2f &get_pos() const { return this->pos; }

    protected:
        Position2f pos;
};

struct MouseMovedEvent: public MouseEvent {
    DECL_EVENT_TYPE_GETTERS(MouseMoved)

    inline MouseMovedEvent(float x, float y): MouseEvent(x, y) { }
};

struct MouseScrolledEvent: public MouseEvent {
    DECL_EVENT_TYPE_GETTERS(MouseScrolled)

    inline MouseScrolledEvent(float x, float y): MouseEvent(x, y) { } // Cheat, x and y aren't actually mouse coordinates
};

struct MouseButtonPressedEvent: public KeyEvent {
    DECL_EVENT_TYPE_GETTERS(MouseButtonPressed)

    inline MouseButtonPressedEvent(int key, int mods): KeyEvent(key, mods) { }
};

struct MouseButtonHeldEvent: public KeyEvent {
    DECL_EVENT_TYPE_GETTERS(MouseButtonHeld)

    inline MouseButtonHeldEvent(int key, int mods): KeyEvent(key, mods) {
        ++this->key_cnts[key];
    }

    inline int        get_repeats() const  { return key_cnts[get_key()]; }
    static inline int get_repeats(int key) { return key_cnts[key]; }

    static inline void reset_key(int key) { key_cnts[key] = 0; }

    protected:
        static inline std::array<std::uint32_t, MouseButtons::Last> key_cnts;
};

struct MouseButtonReleasedEvent: public KeyEvent {
    DECL_EVENT_TYPE_GETTERS(MouseButtonReleased)

    inline MouseButtonReleasedEvent(int key, int mods): KeyEvent(key, mods) {
        MouseButtonHeldEvent::reset_key(key);
    }
};

struct WindowResizedEvent: public Event {
    DECL_EVENT_TYPE_GETTERS(WindowResized)

    inline WindowResizedEvent(int w, int h): area(w, h) { }

    inline int get_w() const { return this->area.w; }
    inline int get_h() const { return this->area.h; }
    inline const Areai &get_area() const { return this->area; }

    protected:
        Areai area;
};

struct WindowMovedEvent: public Event {
    DECL_EVENT_TYPE_GETTERS(WindowMoved)

    inline WindowMovedEvent(int x, int y): pos(x, y) { }

    inline int get_x() const { return this->pos.x; }
    inline int get_y() const { return this->pos.y; }
    inline const Position2i &get_pos() const { return this->pos; }

    protected:
        Position2i pos;
};

struct WindowFocusedEvent: public Event {
    DECL_EVENT_TYPE_GETTERS(WindowFocused)

    inline WindowFocusedEvent() = default;
};

struct WindowDefocusedEvent: public Event {
    DECL_EVENT_TYPE_GETTERS(WindowDefocused)

    WindowDefocusedEvent() = default;
};

struct WindowClosedEvent: public Event {
    DECL_EVENT_TYPE_GETTERS(WindowClosed)

    inline WindowClosedEvent() = default;
};

struct JoystickMovedEvent: public Event {
    DECL_EVENT_TYPE_GETTERS(JoystickMoved)

    inline JoystickMovedEvent(float x, float y, bool is_left): pos(x, y), id(is_left) { }

    inline bool is_left()  const { return  this->id; }
    inline bool is_right() const { return !this->id; }

    inline float get_x() const { return this->pos.x; }
    inline float get_y() const { return this->pos.y; }
    inline const Position2f &get_pos() const { return this->pos; }

    protected:
        Position2f pos;
        bool id;
};

#undef DECL_EVENT_TYPE_GETTERS

class InputManager {
    public:
        template <typename T = Event>
        using Callback = std::function<void(T &)>;

        InputManager() {
#ifdef CMW_SWITCH
            hidInitialize();
#endif
        }

        ~InputManager() {
#ifdef CMW_SWITCH
            hidExit();
#endif
        }

        void set_window(GLFWwindow *window);

        template <typename T>
        std::size_t register_callback(Callback<T> cb) {
            this->callbacks[(int)T::get_static_type()].insert({this->cur_handle, *(Callback<> *)&cb});
            return this->cur_handle++;
        }

        template <typename T>
        void remove_callback(std::size_t handle) {
            this->callbacks[(int)T::get_static_type()].erase(handle);
        }

        template <typename T>
        void process(T event) const {
            for (const auto &[id, cb]: this->callbacks[(int)event.get_type()])
                cb(event);
        }

#ifndef CMW_SWITCH
        void set_window(GLFWwindow *window) const;
#endif

#ifdef CMW_SWITCH
        void process_nx_events(GLFWwindow *window) const;
#endif

    private:
        static void keys_cb(GLFWwindow *window, int key, int scancode, int action, int modifiers);
        static void char_cb(GLFWwindow *window, unsigned int codepoint);
        static void cursor_cb(GLFWwindow *window, double x, double y);
        static void scroll_cb(GLFWwindow *window, double x, double y);
        static void click_cb(GLFWwindow *window, int key, int action, int modifiers);
        static void window_pos_cb(GLFWwindow *window, int x, int y);
        static void window_size_cb(GLFWwindow *window, int width, int height);
        static void window_focus_cb(GLFWwindow *window, int focused);
        static void window_close_cb(GLFWwindow *window);

    protected:
        std::size_t cur_handle = 0;
        std::array<std::unordered_map<std::size_t, Callback<>>, (std::size_t)EventType::Max> callbacks;
#ifdef CMW_SWITCH
        static constexpr float key_held_threshold = 0.5f; // Time (s) a key to to be held to begin firing KeyHeldEvents
#endif
};

} // namespace cmw::input
