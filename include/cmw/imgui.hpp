#pragma once

#include <vector>
#include <memory>
#include <imgui.hpp>
#include <imgui_impl_opengl3.h>

#include "gl/window.hpp"
#include "gl/input.hpp"
#include "log.hpp"
#include "platform.h"

namespace cmw::imgui {

namespace {

float g_last_time = 0.0f;
std::shared_ptr<Window> window = nullptr;

#ifdef CMW_SWITCH

void touchscreen_press_cb(ScreenPressedEvent &e) {
    ImGuiIO& io = ImGui::GetIO();
    io.MousePos = ImVec2(e.get_x(), e.get_y());
    io.MouseDown[0] = true;
}

void touchscreen_touch_cb(ScreenTouchedEvent &e) {
    ImGuiIO& io = ImGui::GetIO();
    io.MousePos = ImVec2(e.get_x(), e.get_y());
    io.MouseDown[0] = true;
}

void touchscreen_release_cb(ScreenReleasedEvent &e) {
    ImGuiIO& io = ImGui::GetIO();
    io.MousePos = ImVec2(e.get_x(), e.get_y());
    io.MouseDown[0] = false;
}

void key_press_cb(KeyPressedEvent &e) {
    ImGuiIO& io = ImGui::GetIO();
    io.KeysDown[e.get_key()] = true;
    if      (e.get_key() == CMW_SWITCH_KEY_L)  io.KeyCtrl  = true;
    else if (e.get_key() == CMW_SWITCH_KEY_ZL) io.KeyShift = true;
    else if (e.get_key() == CMW_SWITCH_KEY_R)  io.KeyAlt   = true;
}

void key_release_cb(KeyReleasedEvent &e) {
    ImGuiIO& io = ImGui::GetIO();
    io.KeysDown[e.get_key()] = false;
    if      (e.get_key() == CMW_SWITCH_KEY_L)  io.KeyCtrl  = false;
    else if (e.get_key() == CMW_SWITCH_KEY_ZL) io.KeyShift = false;
    else if (e.get_key() == CMW_SWITCH_KEY_R)  io.KeyAlt   = false;
}

#else // CMW_SWITCH

void mouse_pos_cb(MouseMovedEvent &e) {
    ImGuiIO& io = ImGui::GetIO();
    io.MousePos = ImVec2(e.get_x(), e.get_y());
}

void mouse_scroll_cb(MouseScrolledEvent &e) {
    ImGuiIO& io = ImGui::GetIO();
    io.MouseWheelH += e.get_x();
    io.MouseWheel  += e.get_y();
}

void mouse_button_press_cb(MouseButtonPressedEvent &e) {
    ImGuiIO& io = ImGui::GetIO();
    if (auto key = e.get_key(); key < 5) // Hardcoded in ImGui example code
        io.MouseDown[key] = true;
}

void mouse_button_release_cb(MouseButtonReleasedEvent &e) {
    ImGuiIO& io = ImGui::GetIO();
    if (auto key = e.get_key(); key < 5) // Hardcoded in ImGui example code
        io.MouseDown[key] = false;
}

void keyboard_key_press_cb(KeyPressedEvent &e) {
    ImGuiIO& io = ImGui::GetIO();
    io.KeysDown[e.get_key()] = true;
}

void keyboard_key_release_cb(KeyReleasedEvent &e) {
    ImGuiIO& io = ImGui::GetIO();
    io.KeysDown[e.get_key()] = false;
}

void keyboard_char_cb(CharTypedEvent &e) {
    ImGuiIO& io = ImGui::GetIO();
    io.AddInputCharacter(e.get_codepoint());
}

#endif // CMW_SWITCH

#if defined(CMW_DEBUG) && CMW_LOG_BACKEND_IS_IMGUI

void draw_log_window(std::vector<char> &logs) {
    if (!ImGui::Begin("Logs", nullptr)) {
        ImGui::End();
        return;
    }

    ImGui::SetWindowPos(ImVec2(10, 10), ImGuiCond_Once);
    ImGui::SetWindowSize(ImVec2(400, 200), ImGuiCond_Once);

    ImGui::BeginChild("scrolling", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);

    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));

    if (logs.size())
        ImGui::TextUnformatted(&*logs.begin(), &*logs.end());

    ImGui::PopStyleVar();

    if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
        ImGui::SetScrollHereY(1.0f);

    ImGui::EndChild();
    ImGui::End();
}

#endif // defined(CMW_DEBUG) && CMW_LOG_BACKEND_IS_IMGUI

} // namespace

void initialize(std::shared_ptr<Window> win) {
    window = win;
    auto [w, h] = window->get_size();

    ImGui::CreateContext();
    ImGui_ImplOpenGL3_Init("#version 330");
    ImGui::StyleColorsDark();

    InputManager *in_man = window->get_input_manager();
#ifdef CMW_SWITCH
    in_man->register_callback<ScreenPressedEvent>(touchscreen_press_cb);
    in_man->register_callback<ScreenTouchedEvent>(touchscreen_touch_cb);
    in_man->register_callback<ScreenReleasedEvent>(touchscreen_release_cb);
    in_man->register_callback<KeyPressedEvent>(key_press_cb);
    in_man->register_callback<KeyReleasedEvent>(key_release_cb);
#else
    in_man->register_callback<MouseMovedEvent>(mouse_pos_cb);
    in_man->register_callback<MouseScrolledEvent>(mouse_scroll_cb);
    in_man->register_callback<MouseButtonPressedEvent>(mouse_button_press_cb);
    in_man->register_callback<MouseButtonReleasedEvent>(mouse_button_release_cb);
    in_man->register_callback<KeyPressedEvent>(keyboard_key_press_cb);
    in_man->register_callback<KeyReleasedEvent>(keyboard_key_release_cb);
    in_man->register_callback<CharTypedEvent>(keyboard_char_cb);
#endif

    ImGuiIO& io = ImGui::GetIO();

    io.IniFilename = nullptr;
    io.BackendPlatformName = "cmw";
    io.DisplaySize = ImVec2(w, h);
    io.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
#ifdef CMW_SWITCH
    io.ConfigFlags |= ImGuiConfigFlags_IsTouchScreen;
#endif

#ifdef CMW_SWITCH
    io.KeyMap[ImGuiKey_LeftArrow]       = CMW_SWITCH_KEY_DLEFT;
    io.KeyMap[ImGuiKey_RightArrow]      = CMW_SWITCH_KEY_DRIGHT;
    io.KeyMap[ImGuiKey_UpArrow]         = CMW_SWITCH_KEY_DUP;
    io.KeyMap[ImGuiKey_DownArrow]       = CMW_SWITCH_KEY_DDOWN;
    io.KeyMap[ImGuiKey_Space]           = CMW_SWITCH_KEY_A;
    io.KeyMap[ImGuiKey_Enter]           = CMW_SWITCH_KEY_X;
    io.KeyMap[ImGuiKey_Escape]          = CMW_SWITCH_KEY_B;
    io.KeyMap[ImGuiKey_Tab]             = CMW_SWITCH_KEY_ZR;
#else
    io.KeyMap[ImGuiKey_Tab]             = CMW_KEY_TAB;
    io.KeyMap[ImGuiKey_LeftArrow]       = CMW_KEY_LEFT;
    io.KeyMap[ImGuiKey_RightArrow]      = CMW_KEY_RIGHT;
    io.KeyMap[ImGuiKey_UpArrow]         = CMW_KEY_UP;
    io.KeyMap[ImGuiKey_DownArrow]       = CMW_KEY_DOWN;
    io.KeyMap[ImGuiKey_PageUp]          = CMW_KEY_PAGE_UP;
    io.KeyMap[ImGuiKey_PageDown]        = CMW_KEY_PAGE_DOWN;
    io.KeyMap[ImGuiKey_Home]            = CMW_KEY_HOME;
    io.KeyMap[ImGuiKey_End]             = CMW_KEY_END;
    io.KeyMap[ImGuiKey_Insert]          = CMW_KEY_INSERT;
    io.KeyMap[ImGuiKey_Delete]          = CMW_KEY_DELETE;
    io.KeyMap[ImGuiKey_Backspace]       = CMW_KEY_BACKSPACE;
    io.KeyMap[ImGuiKey_Space]           = CMW_KEY_SPACE;
    io.KeyMap[ImGuiKey_Enter]           = CMW_KEY_ENTER;
    io.KeyMap[ImGuiKey_Escape]          = CMW_KEY_ESCAPE;
#endif
}

void finalize() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui::DestroyContext();
    window.reset();
}

void begin_frame() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui::NewFrame();

    ImGuiIO& io = ImGui::GetIO();

    float time = (float)glfwGetTime();
    io.DeltaTime = g_last_time > 0.0f ? (time - g_last_time) : (float)(1.0f / 60.0f);
    g_last_time = time;

    // Modifiers are not reliable across systems
#ifndef CMW_SWITCH
    io.KeyCtrl  = io.KeysDown[CMW_KEY_LEFT_CONTROL] || io.KeysDown[CMW_KEY_RIGHT_CONTROL];
    io.KeyShift = io.KeysDown[CMW_KEY_LEFT_SHIFT]   || io.KeysDown[CMW_KEY_RIGHT_SHIFT];
    io.KeyAlt   = io.KeysDown[CMW_KEY_LEFT_ALT]     || io.KeysDown[CMW_KEY_RIGHT_ALT];
    io.KeySuper = io.KeysDown[CMW_KEY_LEFT_SUPER]   || io.KeysDown[CMW_KEY_RIGHT_SUPER];
#endif

#if defined(CMW_DEBUG) && CMW_LOG_BACKEND_IS_IMGUI
    draw_log_window(log::get_logs());
#endif
}

void end_frame() {
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

}