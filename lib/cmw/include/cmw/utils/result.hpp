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

namespace cmw {

class Result {
    public:
        constexpr inline Result() = default;
        constexpr inline Result(uint32_t code): res(code) { }
        constexpr inline Result(uint32_t module, uint32_t desc): res((module & 0x1ff) | ((desc & 0x1fff) << 9)) { }

        constexpr inline explicit operator uint32_t() const { return this->res; }

        constexpr inline uint32_t code()   const { return this->res;}
        constexpr inline uint32_t module() const { return (this->res & 0x1ff) | 2000; }
        constexpr inline uint32_t desc()   const { return (this->res >> 9) & 0x3fff; }

        constexpr inline bool operator==(const Result &rhs) const {
            return this->res == rhs.code();
        }

        constexpr inline bool operator!=(const Result &rhs) const {
            return !(*this == rhs);
        }

        constexpr inline bool operator==(const uint32_t &rhs) const {
            return this->res == rhs;
        }

        constexpr inline bool operator!=(const uint32_t &rhs) const {
            return !(*this == rhs);
        }

        constexpr inline bool succeeded() const {
            return this->res == 0;
        }

        constexpr inline bool failed() const {
            return this->res != 0;
        }

    protected:
        uint32_t res = 0;
};

} // namespace cmw

