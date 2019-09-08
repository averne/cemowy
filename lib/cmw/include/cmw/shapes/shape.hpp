#pragma once

#include <utility>

#include "../color.hpp"
#include "../mesh.hpp"

namespace cmw::shapes {

class Shape {
    public:
        template <typename ...Args>
        Shape(Args &&...args): mesh(std::forward<Args>(args)...) { }

        virtual ~Shape() = default;

        virtual void on_draw() = 0;

        inline Mesh &get_mesh() { return this->mesh; }

        inline Colorf get_blend_color() const { return this->mesh.get_blend_color(); }
        inline void set_blend_color(Colorf color) { this->get_mesh().get_blend_color() = color; }

    protected:
        Mesh mesh;
};

} // namespace cmw::shapes
