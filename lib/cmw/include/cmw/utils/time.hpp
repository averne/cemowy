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
