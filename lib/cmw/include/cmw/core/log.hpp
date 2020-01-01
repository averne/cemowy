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
#include <cstdio>
#include <unistd.h>
#include <string>
#include <chrono>
#include <vector>
#include <atomic>

#include "cmw/platform.h"

// TODO: Log to files asynchronously

#define CMW_LOG_BACKEND_IS_STDOUT (CMW_LOG_BACKEND == CMW_LOG_BACKEND_STDOUT)
#define CMW_LOG_BACKEND_IS_FILE   (CMW_LOG_BACKEND == CMW_LOG_BACKEND_FILE)
#define CMW_LOG_BACKEND_IS_IMGUI  (CMW_LOG_BACKEND == CMW_LOG_BACKEND_IMGUI)

namespace cmw::log {

enum class Level: uint8_t {
    Trace,
    Info,
    Warn,
    Error,
    Fatal,
};

#ifdef CMW_DEBUG

namespace impl {

inline std::atomic<bool> running = false;
inline Level level = Level::Trace;
inline std::chrono::time_point<std::chrono::system_clock> start_time;
constexpr inline const char * lvl_strings[] = {"[TRACE]:", "[INFO]: ", "[WARN]: ", "[ERROR]:", "[FATAL]:"};

#if CMW_LOG_BACKEND_IS_FILE
inline FILE *fp;
#   ifdef CMW_SWITCH
inline std::string filepath = "sdmc:/cmw.log";
#   else
inline std::string filepath = "./cmw.log";
#   endif
#endif

#if CMW_LOG_BACKEND_IS_IMGUI
constexpr inline int logs_max_size = 1000;
inline std::vector<char> logs;
#endif

} // namespace impl

#if CMW_LOG_BACKEND_IS_IMGUI
static inline void add_log(const std::string &str) {
    impl::logs.reserve(impl::logs.size() + str.size() - 1); // Ignore null terminator
    std::copy(&*str.begin(), &*str.end() - 1, std::back_inserter(impl::logs));
}
#endif

#if defined(CMW_SWITCH) && CMW_LOG_BACKEND_IS_FILE
static inline void set_filepath(const std::string &path) {
    impl::filepath = path;
}
#endif

static inline bool is_running() {
    return impl::running;
}

static inline void set_log_level(Level lvl) {
    impl::level = lvl;
}

static inline Level get_log_lvl() {
    return impl::level;
}

#if CMW_LOG_BACKEND_IS_IMGUI
static inline std::vector<char> &get_logs() {
    return impl::logs;
}
#endif

static inline uint32_t initialize() {
    if (is_running())
        return 0x224; // err::AlreadyActve
    impl::start_time = std::chrono::system_clock::now();

#if defined(CMW_SWITCH) && CMW_LOG_BACKEND_IS_STDOUT
    socketInitializeDefault();
    nxlinkStdio();
#endif

#if CMW_LOG_BACKEND_IS_FILE
    if (!(impl::fp = fopen(impl::filepath.c_str(), "w")))
        return 0x1a4;                         // err::FailedFopen
    dup2(fileno(impl::fp), 1);                // Redirect stdout to file
    setvbuf(impl::fp, NULL, _IOLBF, 0x50);    // Flush on newline
#endif

    impl::running = true;
    return 0;
}

static inline uint32_t finalize() {
    if (!is_running())
        return 0x2a4; // err::AlreadyInactive
    impl::running = false;

#if defined(CMW_SWITCH) && CMW_LOG_BACKEND_IS_STDOUT
    socketExit();
#endif

#if CMW_LOG_BACKEND_IS_FILE
    fclose(impl::fp);
#endif

    return 0;
}

template<typename ...Args>
static inline void enqueue(Level lvl, const std::string &fmt, Args &&...args) {
    if (lvl < impl::level || !is_running())
        return;
    std::string format = "[%#.3fs] %s " + fmt;

#if CMW_LOG_BACKEND_IS_STDOUT || CMW_LOG_BACKEND_IS_FILE
    printf(format.c_str(),
        std::chrono::duration_cast<std::chrono::duration<float, std::ratio<1, 1000>>>(std::chrono::system_clock::now() - impl::start_time).count() / 1000.0f,
        impl::lvl_strings[(int)lvl],
        std::forward<Args>(args)...
    );
#endif

#if CMW_LOG_BACKEND_IS_IMGUI
    std::string str(snprintf(NULL, 0, format.c_str(),
        std::chrono::duration_cast<std::chrono::duration<float, std::ratio<1, 1000>>>(std::chrono::system_clock::now() - impl::start_time).count() / 1000.0f,
        impl::lvl_strings[(int)lvl],
        std::forward<Args>(args)...) + 1, 0);
    snprintf(str.data(), str.size(), format.c_str(),
        std::chrono::duration_cast<std::chrono::duration<float, std::ratio<1, 1000>>>(std::chrono::system_clock::now() - impl::start_time).count() / 1000.0f,
        impl::lvl_strings[(int)lvl],
        std::forward<Args>(args)...);
    add_log(str);
#endif
}

#else // CMW_DEBUG

static inline bool is_running() { return false; }
static inline void set_log_level(Level lvl) { }
static inline Level get_log_lvl() { return Level::Fatal; }

static uint32_t initialize() {
    return 0;
}

static uint32_t finalize() {
    return 0;
}

template<typename ...Args>
static inline void enqueue(Level level, const std::string &fmt, Args &&...args) { }

#endif // CMW_DEBUG

} // namespace cmw::log

#define CMW_TRACE(fmt, ...) ::cmw::log::enqueue(::cmw::log::Level::Trace, fmt, ##__VA_ARGS__)
#define CMW_INFO(fmt,  ...) ::cmw::log::enqueue(::cmw::log::Level::Info,  fmt, ##__VA_ARGS__)
#define CMW_WARN(fmt,  ...) ::cmw::log::enqueue(::cmw::log::Level::Warn,  fmt, ##__VA_ARGS__)
#define CMW_ERROR(fmt, ...) ::cmw::log::enqueue(::cmw::log::Level::Error, fmt, ##__VA_ARGS__)
#define CMW_FATAL(fmt, ...) ::cmw::log::enqueue(::cmw::log::Level::Fatal, fmt, ##__VA_ARGS__)
