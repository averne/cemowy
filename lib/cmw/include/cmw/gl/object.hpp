#pragma once

#include <glad/glad.h>

#include "../platform.h"
#include "../utils.hpp"

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

} // namespace cmw
