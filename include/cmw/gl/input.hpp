#pragma once

#include <cstdint>
#include <array>
#include <map>
#include <functional>
#include <utility>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "../utils.hpp"
#include "../platform.h"

// From glfw3.hpp
#define CMW_KEY_UNKNOWN            -1

#define CMW_KEY_SPACE              32
#define CMW_KEY_APOSTROPHE         39  /* ' */
#define CMW_KEY_COMMA              44  /* , */
#define CMW_KEY_MINUS              45  /* - */
#define CMW_KEY_PERIOD             46  /* . */
#define CMW_KEY_SLASH              47  /* / */
#define CMW_KEY_0                  48
#define CMW_KEY_1                  49
#define CMW_KEY_2                  50
#define CMW_KEY_3                  51
#define CMW_KEY_4                  52
#define CMW_KEY_5                  53
#define CMW_KEY_6                  54
#define CMW_KEY_7                  55
#define CMW_KEY_8                  56
#define CMW_KEY_9                  57
#define CMW_KEY_SEMICOLON          59  /* ; */
#define CMW_KEY_EQUAL              61  /* = */
#define CMW_KEY_A                  65
#define CMW_KEY_B                  66
#define CMW_KEY_C                  67
#define CMW_KEY_D                  68
#define CMW_KEY_E                  69
#define CMW_KEY_F                  70
#define CMW_KEY_G                  71
#define CMW_KEY_H                  72
#define CMW_KEY_I                  73
#define CMW_KEY_J                  74
#define CMW_KEY_K                  75
#define CMW_KEY_L                  76
#define CMW_KEY_M                  77
#define CMW_KEY_N                  78
#define CMW_KEY_O                  79
#define CMW_KEY_P                  80
#define CMW_KEY_Q                  81
#define CMW_KEY_R                  82
#define CMW_KEY_S                  83
#define CMW_KEY_T                  84
#define CMW_KEY_U                  85
#define CMW_KEY_V                  86
#define CMW_KEY_W                  87
#define CMW_KEY_X                  88
#define CMW_KEY_Y                  89
#define CMW_KEY_Z                  90
#define CMW_KEY_LEFT_BRACKET       91  /* [ */
#define CMW_KEY_BACKSLASH          92  /* \ */
#define CMW_KEY_RIGHT_BRACKET      93  /* ] */
#define CMW_KEY_GRAVE_ACCENT       96  /* ` */
#define CMW_KEY_WORLD_1            161 /* non-US #1 */
#define CMW_KEY_WORLD_2            162 /* non-US #2 */
#define CMW_KEY_ESCAPE             256
#define CMW_KEY_ENTER              257
#define CMW_KEY_TAB                258
#define CMW_KEY_BACKSPACE          259
#define CMW_KEY_INSERT             260
#define CMW_KEY_DELETE             261
#define CMW_KEY_RIGHT              262
#define CMW_KEY_LEFT               263
#define CMW_KEY_DOWN               264
#define CMW_KEY_UP                 265
#define CMW_KEY_PAGE_UP            266
#define CMW_KEY_PAGE_DOWN          267
#define CMW_KEY_HOME               268
#define CMW_KEY_END                269
#define CMW_KEY_CAPS_LOCK          280
#define CMW_KEY_SCROLL_LOCK        281
#define CMW_KEY_NUM_LOCK           282
#define CMW_KEY_PRINT_SCREEN       283
#define CMW_KEY_PAUSE              284
#define CMW_KEY_F1                 290
#define CMW_KEY_F2                 291
#define CMW_KEY_F3                 292
#define CMW_KEY_F4                 293
#define CMW_KEY_F5                 294
#define CMW_KEY_F6                 295
#define CMW_KEY_F7                 296
#define CMW_KEY_F8                 297
#define CMW_KEY_F9                 298
#define CMW_KEY_F10                299
#define CMW_KEY_F11                300
#define CMW_KEY_F12                301
#define CMW_KEY_F13                302
#define CMW_KEY_F14                303
#define CMW_KEY_F15                304
#define CMW_KEY_F16                305
#define CMW_KEY_F17                306
#define CMW_KEY_F18                307
#define CMW_KEY_F19                308
#define CMW_KEY_F20                309
#define CMW_KEY_F21                310
#define CMW_KEY_F22                311
#define CMW_KEY_F23                312
#define CMW_KEY_F24                313
#define CMW_KEY_F25                314
#define CMW_KEY_KP_0               320
#define CMW_KEY_KP_1               321
#define CMW_KEY_KP_2               322
#define CMW_KEY_KP_3               323
#define CMW_KEY_KP_4               324
#define CMW_KEY_KP_5               325
#define CMW_KEY_KP_6               326
#define CMW_KEY_KP_7               327
#define CMW_KEY_KP_8               328
#define CMW_KEY_KP_9               329
#define CMW_KEY_KP_DECIMAL         330
#define CMW_KEY_KP_DIVIDE          331
#define CMW_KEY_KP_MULTIPLY        332
#define CMW_KEY_KP_SUBTRACT        333
#define CMW_KEY_KP_ADD             334
#define CMW_KEY_KP_ENTER           335
#define CMW_KEY_KP_EQUAL           336
#define CMW_KEY_LEFT_SHIFT         340
#define CMW_KEY_LEFT_CONTROL       341
#define CMW_KEY_LEFT_ALT           342
#define CMW_KEY_LEFT_SUPER         343
#define CMW_KEY_RIGHT_SHIFT        344
#define CMW_KEY_RIGHT_CONTROL      345
#define CMW_KEY_RIGHT_ALT          346
#define CMW_KEY_RIGHT_SUPER        347
#define CMW_KEY_MENU               348


// Switch-specific codes (important -- same order as libnx)
#define CMW_SWITCH_KEY_A           400
#define CMW_SWITCH_KEY_B           401
#define CMW_SWITCH_KEY_X           402
#define CMW_SWITCH_KEY_Y           403
#define CMW_SWITCH_KEY_LSTICK      404
#define CMW_SWITCH_KEY_RSTICK      405
#define CMW_SWITCH_KEY_L           406
#define CMW_SWITCH_KEY_R           407
#define CMW_SWITCH_KEY_ZL          408
#define CMW_SWITCH_KEY_ZR          409
#define CMW_SWITCH_KEY_PLUS        410
#define CMW_SWITCH_KEY_MINUS       411
#define CMW_SWITCH_KEY_DLEFT       412
#define CMW_SWITCH_KEY_DUP         413
#define CMW_SWITCH_KEY_DRIGHT      414
#define CMW_SWITCH_KEY_DDOWN       415

#define CMW_KEY_LAST               CMW_SWITCH_KEY_DDOWN
#define CMW_SWITCH_KEY_FIRST       CMW_SWITCH_KEY_A
#define CMW_SWITCH_KEY_LAST        CMW_SWITCH_KEY_DDOWN

namespace cmw {

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
        static inline std::array<std::uint32_t, GLFW_KEY_LAST> key_cnts;
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
        static inline std::array<std::uint32_t, GLFW_MOUSE_BUTTON_LAST> key_cnts;
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

struct JoystickMovedEvent: public Event, public Position<int32_t> {
    inline JoystickMovedEvent(int32_t x, int32_t y, bool is_left): Position(x, y), id(is_left) { }

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

        InputManager(GLFWwindow *window) {
            s_this = this;
            set_window(window);
#ifdef CMW_SWITCH
            hidInitialize();
#endif
        }

        ~InputManager() {
#ifdef CMW_SWITCH
            hidExit();
#endif
        }

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

        void set_window(GLFWwindow *window) const {
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
        void process_nx_events(GLFWwindow *window) {
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
            {
                JoystickPosition pos;
                hidJoystickRead(&pos, CONTROLLER_P1_AUTO, JOYSTICK_LEFT);
                if ((pos.dx + pos.dy) != 0)
                    this->process(JoystickMovedEvent(pos.dx, pos.dy, 1));
                hidJoystickRead(&pos, CONTROLLER_P1_AUTO, JOYSTICK_RIGHT);
                if ((pos.dx + pos.dy) != 0)
                    this->process(JoystickMovedEvent(pos.dx, pos.dy, 0));
            }
        }
#endif

    private:
        static void keys_cb(GLFWwindow *window, int key, int scancode, int action, int modifiers) {
            if (action == GLFW_PRESS)
                s_this->process(KeyPressedEvent(key, modifiers));
            else if (action == GLFW_RELEASE)
                s_this->process(KeyReleasedEvent(key, modifiers));
            else
                s_this->process(KeyHeldEvent(key, modifiers));
        }

        static void char_cb(GLFWwindow *window, unsigned int codepoint) {
            s_this->process(CharTypedEvent(codepoint));
        }

        static void cursor_cb(GLFWwindow *window, double x, double y) {
            s_this->process(MouseMovedEvent(x, y));
        }

        static void scroll_cb(GLFWwindow *window, double x, double y) {
            s_this->process(MouseScrolledEvent(x, y));
        }

        static void click_cb(GLFWwindow *window, int key, int action, int modifiers) {
            if (action == GLFW_PRESS)
                s_this->process(MouseButtonPressedEvent(key, modifiers));
            else if (action == GLFW_RELEASE)
                s_this->process(MouseButtonReleasedEvent(key, modifiers));
            else
                s_this->process(MouseButtonHeldEvent(key, modifiers));
        }

        static void window_pos_cb(GLFWwindow* window, int x, int y) {
            s_this->process(WindowMovedEvent(x, y));
        }

        static void window_size_cb(GLFWwindow* window, int width, int height) {
            s_this->process(WindowResizedEvent(width, height));
        }

        static void window_focus_cb(GLFWwindow* window, int focused) {
            if (focused)
                s_this->process(WindowFocusedEvent());
            else
                s_this->process(WindowDefocusedEvent());
        }

        static void window_close_cb(GLFWwindow* window) {
            s_this->process(WindowClosedEvent());
        }

    protected:
        static inline InputManager *s_this;
        std::size_t cur_handle = 0;
        std::array<std::map<std::size_t, Callback<>>, (std::size_t)EventType::Max> callbacks;
#ifdef CMW_SWITCH
        static inline constexpr float key_held_threshold = 0.5f; // Time (s) a key to to be held to begin firing KeyHeldEvents
#endif
};

} // namespace cmw
