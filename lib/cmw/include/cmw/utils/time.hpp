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

using namespace std::chrono_literals;

namespace cmw {

template <typename C = std::chrono::system_clock, typename U = std::chrono::milliseconds>
class StopWatch {
    public:
        using Clock = C;
        using Unit  = U;

    public:
        inline StopWatch(): start(Clock::now()) { }
        inline virtual ~StopWatch() = default;

        template <typename T = Unit>
        inline T start_time() const {
            return (T)std::chrono::duration_cast<Unit>(this->start).count();
        }

        template <typename T = Unit>
        inline T elapsed() const {
            return (T)std::chrono::duration_cast<Unit>(Clock::now() - this->start).count();
        }

    protected:
        std::chrono::time_point<Clock> start;
};

template <typename Clock = std::chrono::system_clock, typename Unit = std::chrono::milliseconds>
class Timer: public StopWatch<Clock, Unit> {
    public:
        inline Timer(Unit timeout): timeout(timeout) { }

        inline operator bool() const {
            return timeout.count() > this->template elapsed<typename Unit::rep>();
        }

        template <typename T = Unit>
        inline T remaining() const {
            return (T)(timeout - this->template elapsed<Unit>());
        }

    protected:
        Unit timeout;
};

} // namespace cmw
