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
#include <functional>
#include <utility>
#include <glm/glm.hpp>

#include "cmw/utils/time.hpp"

namespace cmw {

template <typename Ret, typename Param>
class Animation {
    public:
        template <typename F>
        inline Animation(F &&f): f(std::forward<F>(f)) { }
        virtual ~Animation() = default;

        inline Ret update(Param &&p) const {
            return this->f(std::forward<Param>(p));
        }

    protected:
        std::function<Ret(Param)> f;
};

template <typename Ret>
class TemporalAnimation: public Animation<Ret, std::chrono::milliseconds> {
    public:
        using Unit  = std::chrono::milliseconds;
        using Clock = std::chrono::system_clock;

    public:
        template <typename F>
        inline TemporalAnimation(F &&f): Animation<Ret, Unit>(std::forward<F>(f)) { }

        inline Ret update() const {
            return this->template f(watch.elapsed());
        }

    protected:
        StopWatch<Clock, Unit> watch;
};

template <typename Ret>
class TimedAnimation: public Animation<Ret, std::chrono::milliseconds> {
    public:
        using Unit  = std::chrono::milliseconds;
        using Clock = std::chrono::system_clock;

    public:
        template <typename F>
        inline TimedAnimation(Unit timeout, F &&f): Animation<Ret, Unit>(std::forward<F>(f)), timer(timeout) { }

        inline Ret update() const {
            return this->template f(timer.elapsed());
        }

        inline bool is_finished() const {
            return timer;
        }

        inline Unit remaining() {
            return timer.remaining();
        }

        inline operator bool() const {
            return is_finished();
        }

    protected:
        Timer<Clock, Unit> timer;
};

using GeometricAnimation      = TemporalAnimation<glm::mat4>;
using TimedGeometricAnimation = TimedAnimation<glm::mat4>;

template <typename T>
class AnimatedObject {
    public:
        T object;

    public:
        template <typename F, typename ...Args>
        inline AnimatedObject(F f, Args &&...args):
            object(std::forward<Args>(args)...), animation(std::move(f)) { }

        inline glm::mat4 update() const {
            return this->animation.update();
        }

    protected:
        GeometricAnimation animation;
};

// template <typename T>
// class TimedAnimatedObject {
//     public:
//         template <typename F, typename ...Args>
//         inline TimedAnimatedObject(TimedGeometricAnimation::Unit timeout, F &&f, Args &&...args):
//             animation(timeout, std::forward<F>(f)), object(std::forward<Args>(args)...) { }

//         inline glm::mat4 &update() {
//             if (this->animation)
//                 return this->model = this->animation.update();
//             return this->model;
//         }

//     public:
//         T object;

//     protected:
//         TimedGeometricAnimation animation;
//         glm::mat4 model;
// };

} // namespace cmw
