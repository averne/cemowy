#include <chrono>

namespace cmw::time {

template <typename Clock = std::chrono::system_clock>
class StopWatch {
    public:
        StopWatch(): start(Clock::now()) { }
        virtual ~StopWatch() = default;

        template <typename T, typename Unit = std::chrono::milliseconds>
        T start_time() const {
            return (T)std::chrono::duration_cast<Unit>(this->start).count();
        }

        template <typename T, typename Unit = std::chrono::milliseconds>
        T elapsed() const {
            return (T)std::chrono::duration_cast<Unit>(Clock::now() - this->start).count();
        }

    protected:
        std::chrono::time_point<Clock> start;
};

template <typename Unit = std::chrono::milliseconds, typename Clock = std::chrono::system_clock>
class Timer: public StopWatch<Clock> {
    public:
        Timer(Unit timeout): timeout(timeout) { }

        operator bool() const {
            return timeout.count() > this->template elapsed<typename Unit::rep, Unit>();
        }

    protected:
        Unit timeout;
};

} // namespace cmw::time
