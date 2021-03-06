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

#include <vector>
#include <memory>
#include <imgui.hpp>
#include <imgui_impl_opengl3.h>

#include "cmw/core/input.hpp"
#include "cmw/core/log.hpp"
#include "cmw/core/window.hpp"
#include "cmw/platform.h"

namespace cmw::imgui {

#ifdef CMW_DEBUG

namespace impl {

inline float g_last_time = 0.0f;
inline Window *window = nullptr;

static void mouse_pos_cb(input::MouseMovedEvent &e) {
    ImGuiIO &io = ImGui::GetIO();
    auto [w, h] = window->get_size();
    io.MousePos = ImVec2(e.get_x(), h - e.get_y());
}

static void mouse_button_press_cb(input::MouseButtonPressedEvent &e) {
    ImGuiIO &io = ImGui::GetIO();
    if (int key = (int)e.get_key(); key < 5) // Hardcoded in ImGui example code
        io.MouseDown[key] = true;
}

static void mouse_button_release_cb(input::MouseButtonReleasedEvent &e) {
    ImGuiIO &io = ImGui::GetIO();
    if (auto key = e.get_key(); key < 5) // Hardcoded in ImGui example code
        io.MouseDown[key] = false;
}

static void keyboard_key_press_cb(input::KeyPressedEvent &e) {
    ImGuiIO &io = ImGui::GetIO();
    io.KeysDown[e.get_key()] = true;
#ifdef CMW_SWITCH // Emulate mod keys
    if      (e.get_key() == input::KeySwitchL)  io.KeyCtrl  = true;
    else if (e.get_key() == input::KeySwitchZl) io.KeyShift = true;
    else if (e.get_key() == input::KeySwitchR)  io.KeyAlt   = true;
#endif
}

static void keyboard_key_release_cb(input::KeyReleasedEvent &e) {
    ImGuiIO &io = ImGui::GetIO();
    io.KeysDown[e.get_key()] = false;
#ifdef CMW_SWITCH // Emulate mod keys
    if      (e.get_key() == input::KeySwitchL)  io.KeyCtrl  = false;
    else if (e.get_key() == input::KeySwitchZl) io.KeyShift = false;
    else if (e.get_key() == input::KeySwitchR)  io.KeyAlt   = false;
#endif
}

static void mouse_scroll_cb(input::MouseScrolledEvent &e) {
    ImGuiIO &io = ImGui::GetIO();
    io.MouseWheelH += e.get_x();
    io.MouseWheel  += e.get_y();
}

static void keyboard_char_cb(input::CharTypedEvent &e) {
    ImGuiIO &io = ImGui::GetIO();
    io.AddInputCharacter(e.get_codepoint());
}

#if CMW_LOG_BACKEND_IS_IMGUI

static void draw_log_window(std::vector<char> &logs) {
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

#endif // CMW_LOG_BACKEND_IS_IMGUI

} // namespace impl

static inline void initialize(Window *win) {
    impl::window = win;
    auto [w, h] = impl::window->get_size();

    ImGui::CreateContext();
    ImGui::Gl3Impl::Init("#version 430");
    ImGui::StyleColorsDark();

    input::InputManager &in_man = impl::window->get_input_manager();
    in_man.register_callback<input::MouseMovedEvent>(impl::mouse_pos_cb);
    in_man.register_callback<input::MouseScrolledEvent>(impl::mouse_scroll_cb);
    in_man.register_callback<input::MouseButtonPressedEvent>(impl::mouse_button_press_cb);
    in_man.register_callback<input::MouseButtonReleasedEvent>(impl::mouse_button_release_cb);
    in_man.register_callback<input::KeyPressedEvent>(impl::keyboard_key_press_cb);
    in_man.register_callback<input::KeyReleasedEvent>(impl::keyboard_key_release_cb);
    in_man.register_callback<input::CharTypedEvent>(impl::keyboard_char_cb);

    ImGuiIO &io = ImGui::GetIO();

    io.IniFilename = nullptr;
    io.BackendPlatformName = "cmw";
    io.DisplaySize = ImVec2(w, h);
    io.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
#ifdef CMW_SWITCH
    io.ConfigFlags |= ImGuiConfigFlags_IsTouchScreen;
    // io.FontGlobalScale = 1.5f;
#endif

#ifdef CMW_SWITCH
    io.KeyMap[ImGuiKey_LeftArrow]       = input::KeySwitchDleft;
    io.KeyMap[ImGuiKey_RightArrow]      = input::KeySwitchDright;
    io.KeyMap[ImGuiKey_UpArrow]         = input::KeySwitchDup;
    io.KeyMap[ImGuiKey_DownArrow]       = input::KeySwitchDdown;
    io.KeyMap[ImGuiKey_Space]           = input::KeySwitchA;
    io.KeyMap[ImGuiKey_Enter]           = input::KeySwitchX;
    io.KeyMap[ImGuiKey_Escape]          = input::KeySwitchB;
    io.KeyMap[ImGuiKey_Tab]             = input::KeySwitchZr;
#else
    io.KeyMap[ImGuiKey_Tab]             = input::KeyTab;
    io.KeyMap[ImGuiKey_LeftArrow]       = input::KeyLeft;
    io.KeyMap[ImGuiKey_RightArrow]      = input::KeyRight;
    io.KeyMap[ImGuiKey_UpArrow]         = input::KeyUp;
    io.KeyMap[ImGuiKey_DownArrow]       = input::KeyDown;
    io.KeyMap[ImGuiKey_PageUp]          = input::KeyPageUp;
    io.KeyMap[ImGuiKey_PageDown]        = input::KeyPageDown;
    io.KeyMap[ImGuiKey_Home]            = input::KeyHome;
    io.KeyMap[ImGuiKey_End]             = input::KeyEnd;
    io.KeyMap[ImGuiKey_Insert]          = input::KeyInsert;
    io.KeyMap[ImGuiKey_Delete]          = input::KeyDelete;
    io.KeyMap[ImGuiKey_Backspace]       = input::KeyBackspace;
    io.KeyMap[ImGuiKey_Space]           = input::KeySpace;
    io.KeyMap[ImGuiKey_Enter]           = input::KeyEnter;
    io.KeyMap[ImGuiKey_Escape]          = input::KeyEscape;
#endif

#ifdef CMW_SWITCH
    ImGuiStyle &style = ImGui::GetStyle();
    style.ScaleAllSizes(1.5f);
#endif
}

static inline void finalize() {
    ImGui::Gl3Impl::Shutdown();
    ImGui::DestroyContext();
    impl::window = nullptr;
}

static inline void begin_frame() {
    ImGui::Gl3Impl::NewFrame();
    ImGui::NewFrame();

    ImGuiIO &io = ImGui::GetIO();

    float time = (float)glfwGetTime();
    io.DeltaTime = impl::g_last_time > 0.0f ? (time - impl::g_last_time) : (float)(1.0f / 60.0f);
    impl::g_last_time = time;

    // Modifiers are not reliable across systems
#ifndef CMW_SWITCH
    io.KeyCtrl  = io.KeysDown[input::KeyLeftControl] || io.KeysDown[input::KeyRightControl];
    io.KeyShift = io.KeysDown[input::KeyLeftShift]   || io.KeysDown[input::KeyRightShift];
    io.KeyAlt   = io.KeysDown[input::KeyLeftAlt]     || io.KeysDown[input::KeyRightAlt];
    io.KeySuper = io.KeysDown[input::KeyLeftSuper]   || io.KeysDown[input::KeyRightSuper];
#endif

#if CMW_LOG_BACKEND_IS_IMGUI
    impl::draw_log_window(log::get_logs());
#endif
}

static inline void end_frame() {
    ImGui::Render();
    ImGui::Gl3Impl::RenderDrawData(ImGui::GetDrawData());
}

#else // CMW_DEBUG

static void initialize(Window *) { }
static void finalize() { }
static void begin_frame() { }
static void end_frame() { }

#endif // CMW_DEBUG

} // namespace cmw::imgui
