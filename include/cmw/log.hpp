#pragma once

#include <cstdint>
#include <cstdio>
#include <string>
#include <chrono>

#include "platform.h"

namespace cmw {

// TODO: Implement other backends (stdout, file, imgui) according to compile-time defines

class Logger {
    public:
        enum class Level: uint8_t { // Aliases used in the macro below
            Trace, trace = Trace,
            Info,  info  = Info,
            Warn,  warn  = Warn,
            Error, error = Error,
            Fatal, fatal = Fatal,
        };

        Logger()  = delete;
        ~Logger() = delete;

        // Can't use macros here
        Logger(const Logger &)            = delete;
        Logger &operator=(const Logger &) = delete;
        Logger(Logger &&)                 = delete;
        Logger &operator=(Logger &&)      = delete;

#ifdef CMW_SWITCH
        static void set_filepath(const std::string &path) {
            filepath = path;
        }
#endif

        static uint32_t initialize() {
#ifdef CMW_DEBUG
            if (is_running())
                return 0x224; // err::AlreadyActve
            start_time = std::chrono::system_clock::now();
#ifdef CMW_SWITCH
            if (!(fp = fopen(filepath.c_str(), "w")))
                return 0x1a4; // err::FailedFopen
            dup2(fileno(fp), 1);             // Redirect stdout to file
            setvbuf(fp, NULL, _IOLBF, 0x50); // Flush on newline
#endif // CMW_SWITCH
            running = true;
#endif // CMW_DEBUG
            return 0;
        }

        static uint32_t finalize() {
#ifdef CMW_DEBUG
            if (!is_running())
                return 0x2a4; // err::AlreadyInactive
            running = false;
#ifdef CMW_SWITCH
            fclose(fp);
#endif // CMW_SWITCH
#endif // CMW_DEBUG
            return 0;
        }

        static inline bool is_running() { return running; }
        static inline void set_log_level(Level lvl) { level = lvl; }
        static inline Level get_log_lvl() { return level; }

        template<typename ...Args>
        static inline void enqueue(Level lvl, const char *fmt, Args &&...args) {
#ifdef CMW_DEBUG
            if (lvl < level || !is_running())
                return;
            std::string fmted(snprintf(nullptr, 0, fmt, args...) + 1, 0);
            snprintf((char *)fmted.data(), fmted.size(), fmt, args...);
            printf("[%#.3fs] %s %s",
                std::chrono::duration_cast<std::chrono::duration<float, std::ratio<1, 1000>>>(std::chrono::system_clock::now() - start_time).count() / 1000.0f,
                lvl_strings[(int)lvl],
                fmted.c_str()
            );
#endif
        }

        template<typename ...Args>
        static inline void enqueue(Level lvl, const std::string &fmt, Args &&...args) {
            enqueue(lvl, fmt.c_str(), std::forward<Args>(args)...);
        }

    protected:
#ifdef DEBUG
        static inline bool running = false;
        static inline Level level = Level::Trace;
        static inline std::chrono::time_point<std::chrono::system_clock> start_time;
#ifdef CMW_SWITCH
        static inline std::string filepath = "sdmc:/cmw.log";
        static inline FILE *fp;
#endif // CMW_SWITCH
        static inline const char * lvl_strings[] = {"[TRACE]:", "[INFO]: ", "[WARN]: ", "[ERROR]:", "[FATAL]:"};
#endif // CMW_DEBUG
};

#ifdef CMW_DEBUG
#   define CMW_TRACE(fmt, ...) ::cmw::Logger::enqueue(::cmw::Logger::Level::Trace, fmt, ##__VA_ARGS__)
#   define CMW_INFO(fmt,  ...) ::cmw::Logger::enqueue(::cmw::Logger::Level::Info,  fmt, ##__VA_ARGS__)
#   define CMW_WARN(fmt,  ...) ::cmw::Logger::enqueue(::cmw::Logger::Level::Warn,  fmt, ##__VA_ARGS__)
#   define CMW_ERROR(fmt, ...) ::cmw::Logger::enqueue(::cmw::Logger::Level::Error, fmt, ##__VA_ARGS__)
#   define CMW_FATAL(fmt, ...) ::cmw::Logger::enqueue(::cmw::Logger::Level::Fatal, fmt, ##__VA_ARGS__)
#else
#   define CMW_TRACE(fmt, ...) ({})
#   define CMW_INFO(fmt,  ...) ({})
#   define CMW_WARN(fmt,  ...) ({})
#   define CMW_ERROR(fmt, ...) ({})
#   define CMW_FATAL(fmt, ...) ({})
#endif

} // namespawe cmw
