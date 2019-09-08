#pragma once

#include <cstdint>
#include <array>
#include <map>
#include <functional>
#include <utility>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "utils.hpp"
#include "platform.h"

namespace cmw::input {

// Same as GLFW's
enum MouseButtons: std::uint16_t {
    Button1              = 0,
    Button2              = 1,
    Button3              = 2,
    Button4              = 3,
    Button5              = 4,
    Button6              = 5,
    Button7              = 6,
    Button8              = 7,

    Left                 = Button1,
    Right                = Button2,
    Middle               = Button3,

    First                = Button1,
    Last                 = Button8,
};

enum Keys: std::uint16_t {
    KeyUnknown           = std::numeric_limits<std::uint16_t>::max(),

    KeySpace             = 32,
    KeyApostrophe        = 39,  /* ' */
    KeyComma             = 44,  /* , */
    KeyMinus             = 45,  /* - */
    KeyPeriod            = 46,  /* . */
    KeySlash             = 47,  /* / */
    Key0                 = 48,
    Key1                 = 49,
    Key2                 = 50,
    Key3                 = 51,
    Key4                 = 52,
    Key5                 = 53,
    Key6                 = 54,
    Key7                 = 55,
    Key8                 = 56,
    Key9                 = 57,
    KeySemicolon         = 59,  /* ; */
    KeyEqual             = 61,  /* = */
    KeyA                 = 65,
    KeyB                 = 66,
    KeyC                 = 67,
    KeyD                 = 68,
    KeyE                 = 69,
    KeyF                 = 70,
    KeyG                 = 71,
    KeyH                 = 72,
    KeyI                 = 73,
    KeyJ                 = 74,
    KeyK                 = 75,
    KeyL                 = 76,
    KeyM                 = 77,
    KeyN                 = 78,
    KeyO                 = 79,
    KeyP                 = 80,
    KeyQ                 = 81,
    KeyR                 = 82,
    KeyS                 = 83,
    KeyT                 = 84,
    KeyU                 = 85,
    KeyV                 = 86,
    KeyW                 = 87,
    KeyX                 = 88,
    KeyY                 = 89,
    KeyZ                 = 90,
    KeyLeftBracket       = 91,  /* [ */
    KeyBackslash         = 92,  /* \ */
    KeyRightBracket      = 93,  /* ] */
    KeyGraveAccent       = 96,  /* ` */
    KeyWorld1            = 161, /* non-US #1 */
    KeyWorld2            = 162, /* non-US #2 */
    KeyEscape            = 256,
    KeyEnter             = 257,
    KeyTab               = 258,
    KeyBackspace         = 259,
    KeyInsert            = 260,
    KeyDelete            = 261,
    KeyRight             = 262,
    KeyLeft              = 263,
    KeyDown              = 264,
    KeyUp                = 265,
    KeyPageUp            = 266,
    KeyPageDown          = 267,
    KeyHome              = 268,
    KeyEnd               = 269,
    KeyCapsLock          = 280,
    KeyScrollLock        = 281,
    KeyNumLock           = 282,
    KeyPrintScreen       = 283,
    KeyPause             = 284,
    KeyF1                = 290,
    KeyF2                = 291,
    KeyF3                = 292,
    KeyF4                = 293,
    KeyF5                = 294,
    KeyF6                = 295,
    KeyF7                = 296,
    KeyF8                = 297,
    KeyF9                = 298,
    KeyF10               = 299,
    KeyF11               = 300,
    KeyF12               = 301,
    KeyF13               = 302,
    KeyF14               = 303,
    KeyF15               = 304,
    KeyF16               = 305,
    KeyF17               = 306,
    KeyF18               = 307,
    KeyF19               = 308,
    KeyF20               = 309,
    KeyF21               = 310,
    KeyF22               = 311,
    KeyF23               = 312,
    KeyF24               = 313,
    KeyF25               = 314,
    KeyKp0               = 320,
    KeyKp1               = 321,
    KeyKp2               = 322,
    KeyKp3               = 323,
    KeyKp4               = 324,
    KeyKp5               = 325,
    KeyKp6               = 326,
    KeyKp7               = 327,
    KeyKp8               = 328,
    KeyKp9               = 329,
    KeyKpDecimal         = 330,
    KeyKpDivide          = 331,
    KeyKpMultiply        = 332,
    KeyKpSubstract       = 333,
    KeyKpAdd             = 334,
    KeyKpEnter           = 335,
    KeyKpEqual           = 336,
    KeyLeftShift         = 340,
    KeyLeftControl       = 341,
    KeyLeftAlt           = 342,
    KeyLeftSuper         = 343,
    KeyRightShift        = 344,
    KeyRightControl      = 345,
    KeyRightAlt          = 346,
    KeyRightSuper        = 347,
    KeyMenu              = 348,

    // Switch-specific codes (important -- same order as libnx)
    KeySwitchA           = 400,
    KeySwitchB           = 401,
    KeySwitchX           = 402,
    KeySwitchY           = 403,
    KeySwitchLstick      = 404,
    KeySwitchRstick      = 405,
    KeySwitchL           = 406,
    KeySwitchR           = 407,
    KeySwitchZl          = 408,
    KeySwitchZr          = 409,
    KeySwitchPlus        = 410,
    KeySwitchMinus       = 411,
    KeySwitchDleft       = 412,
    KeySwitchDup         = 413,
    KeySwitchDright      = 414,
    KeySwitchDdown       = 415,

    KeyFirst             = KeySpace,
    KeyLast              = KeySwitchDdown,
    KeySwitchFirst       = KeySwitchA,
    KeySwitchLast        = KeySwitchDdown,

    ModShift             = CMW_BIT(0),
    ModControl           = CMW_BIT(1),
    ModAlt               = CMW_BIT(2),
    ModSuper             = CMW_BIT(3),
    ModCapsLock          = CMW_BIT(4),
    ModNumLock           = CMW_BIT(5),
};

enum class EventType: uint8_t {
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

struct Event { };

struct KeyEvent: public Event {
    inline KeyEvent(int key, int mods): key(key), mods(mods) { }

    inline int get_key()  const { return this->key; }
    inline int get_mods() const { return this->mods; }

    inline bool has_mod(int mod) const { return get_mods() & mod; }

    protected:
        int key, mods;
};

struct KeyPressedEvent: public KeyEvent {
    inline KeyPressedEvent(int key, int mods): KeyEvent(key, mods) { }

    static inline constexpr EventType get_type() { return EventType::KeyPressed; };
};

struct KeyHeldEvent: public KeyEvent {
    inline KeyHeldEvent(int key, int mods): KeyEvent(key, mods) {
        ++this->key_cnts[key];
    }

    inline int        get_repeats() const  { return key_cnts[get_key()]; }
    static inline int get_repeats(int key) { return key_cnts[key]; }

    static inline void reset_key(int key) { key_cnts[key] = 0; }

    static inline constexpr EventType get_type() { return EventType::KeyHeld; };

    protected:
        static inline std::array<std::uint32_t, Keys::KeyLast> key_cnts;
};

struct KeyReleasedEvent: public KeyEvent {
    inline KeyReleasedEvent(int key, int mods): KeyEvent(key, mods) {
        KeyHeldEvent::reset_key(key);
    }

    static inline constexpr EventType get_type() { return EventType::KeyReleased; };
};

struct CharTypedEvent: public Event {
    inline CharTypedEvent(unsigned int codepoint): codepoint(codepoint) { }

    inline unsigned int get_codepoint() const { return this->codepoint; }

    static inline constexpr EventType get_type() { return EventType::CharTyped; };

    protected:
        unsigned int codepoint;
};

struct MouseEvent: public Event, Position<float> {
    inline MouseEvent(float x, float y): Position(x, y) { }
};

struct MouseMovedEvent: public MouseEvent {
    inline MouseMovedEvent(float x, float y): MouseEvent(x, y) { }

    static inline constexpr EventType get_type() { return EventType::MouseMoved; };
};

struct MouseScrolledEvent: public MouseEvent {
    inline MouseScrolledEvent(float x, float y): MouseEvent(x, y) { } // Cheat, x and y aren't actually mouse coordinates

    static inline constexpr EventType get_type() { return EventType::MouseScrolled; };
};

struct MouseButtonPressedEvent: public KeyEvent {
    inline MouseButtonPressedEvent(int key, int mods): KeyEvent(key, mods) { }

    static inline constexpr EventType get_type() { return EventType::MouseButtonPressed; };
};

struct MouseButtonHeldEvent: public KeyEvent {
    inline MouseButtonHeldEvent(int key, int mods): KeyEvent(key, mods) {
        ++this->key_cnts[key];
    }

    inline int        get_repeats() const  { return key_cnts[get_key()]; }
    static inline int get_repeats(int key) { return key_cnts[key]; }

    static inline void reset_key(int key) { key_cnts[key] = 0; }

    static inline constexpr EventType get_type() { return EventType::MouseButtonHeld; };

    protected:
        static inline std::array<std::uint32_t, MouseButtons::Last> key_cnts;
};

struct MouseButtonReleasedEvent: public KeyEvent {
    inline MouseButtonReleasedEvent(int key, int mods): KeyEvent(key, mods) {
        MouseButtonHeldEvent::reset_key(key);
    }

    static inline constexpr EventType get_type() { return EventType::MouseButtonReleased; };
};

struct WindowResizedEvent: public Event, Area<int> {
    inline WindowResizedEvent(int w, int h): Area(w, h) { }

    static inline constexpr EventType get_type() { return EventType::WindowResized; }
};

struct WindowMovedEvent: public Event, Position<int> {
    inline WindowMovedEvent(int x, int y): Position(x, y) { }

    static inline constexpr EventType get_type() { return EventType::WindowMoved; }
};

struct WindowFocusedEvent: public Event {
    inline WindowFocusedEvent() = default;

    static inline constexpr EventType get_type() { return EventType::WindowFocused; }
};

struct WindowDefocusedEvent: public Event {
    WindowDefocusedEvent() = default;

    static inline constexpr EventType get_type() { return EventType::WindowDefocused; }
};

struct WindowClosedEvent: public Event {
    inline WindowClosedEvent() = default;

    static inline constexpr EventType get_type() { return EventType::WindowClosed; }
};

struct JoystickMovedEvent: public Event, public Position<float> {
    inline JoystickMovedEvent(float x, float y, bool is_left): Position(x, y), id(is_left) { }

    inline bool is_left()  const { return  this->id; }
    inline bool is_right() const { return !this->id; }

    static inline constexpr EventType get_type() { return EventType::JoystickMoved; }

    protected:
        bool id;
};

struct TouchscreenEvent: public Event, public Position<uint32_t> {
    inline TouchscreenEvent(uint32_t x, uint32_t y, uint32_t dx, uint32_t dy, uint32_t angle):
            Position(x, y), dx(dx), dy(dy), angle(angle) { }

    inline uint32_t get_dx()    const { return this->dx; }
    inline uint32_t get_dy()    const { return this->dy; }
    inline uint32_t get_angle() const { return this->angle; }

    protected:
        uint32_t dx, dy, angle;
};

struct ScreenPressedEvent: public TouchscreenEvent {
    inline ScreenPressedEvent(uint32_t x, uint32_t y, uint32_t dx, uint32_t dy, uint32_t angle):
            TouchscreenEvent(x, y, dx, dy, angle) { }

    static inline constexpr EventType get_type() { return EventType::ScreenPressed; }
};

struct ScreenTouchedEvent: public TouchscreenEvent {
    inline ScreenTouchedEvent(uint32_t x, uint32_t y, uint32_t dx, uint32_t dy, uint32_t angle):
            TouchscreenEvent(x, y, dx, dy, angle) { }

    static inline constexpr EventType get_type() { return EventType::ScreenTouched; }
};

struct ScreenReleasedEvent: public TouchscreenEvent {
    inline ScreenReleasedEvent(uint32_t x, uint32_t y, uint32_t dx, uint32_t dy, uint32_t angle):
            TouchscreenEvent(x, y, dx, dy, angle) { }

    static inline constexpr EventType get_type() { return EventType::ScreenReleased; }
};

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
            this->callbacks[(int)T::get_type()].insert({this->cur_handle, *(Callback<> *)&cb});
            return this->cur_handle++;
        }

        template <typename T>
        void remove_callback(std::size_t handle) {
            this->callbacks[(int)T::get_type()].erase(handle);
        }

        template <typename T>
        void process(T event) const {
            for (auto &[id, cb]: this->callbacks[(int)event.get_type()])
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
        std::array<std::map<std::size_t, Callback<>>, (std::size_t)EventType::Max> callbacks;
#ifdef CMW_SWITCH
        static constexpr float key_held_threshold = 0.5f; // Time (s) a key to to be held to begin firing KeyHeldEvents
#endif
};

} // namespace cmw::input
