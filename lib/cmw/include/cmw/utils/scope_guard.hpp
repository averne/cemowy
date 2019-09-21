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

#include "cmw/utils.hpp"

namespace cmw {

class ScopeGuard {
    CMW_NON_COPYABLE(ScopeGuard);
    CMW_NON_MOVEABLE(ScopeGuard);

    public:
        template <typename T>
        [[nodiscard]] static ScopeGuard create(T &&f) {
            return ScopeGuard(std::forward<T>(f));
        }

        ~ScopeGuard() {
            if (this->f)
                run();
        }

        void run() {
            this->f();
            dismiss();
        }

        void dismiss() {
            f = nullptr;
        }

    private:
        template<class T>
        ScopeGuard(T &&f): f(std::forward<T>(f)) { }

    private:
        std::function<void()> f;
};

} // namespace cmw

#define CMW_SCOPE_GUARD(x) auto CMW_ANONYMOUS_VAR = ::cmw::ScopeGuard::create(x)
