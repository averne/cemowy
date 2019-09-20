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

#include <glad/glad.h>

#include "cmw/utils.hpp"

namespace cmw::gl {

class GlObject {
    CMW_NON_COPYABLE(GlObject);
    CMW_NON_MOVEABLE(GlObject);

    public:
        GlObject() = default;
        GlObject(GLuint handle): handle(handle) { }

        virtual ~GlObject() = default;

        inline GLuint get_handle() const { return this->handle; }

        inline bool operator==(GlObject &rhs) const {
            return get_handle() == rhs.get_handle();
        }

        inline bool operator!=(GlObject &rhs) const {
            return !(*this == rhs);
        }

    protected:
        GLuint handle = 0;
};

} // namespace cmw::gl
