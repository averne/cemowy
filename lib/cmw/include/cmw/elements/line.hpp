#pragma once

#include <vector>
#include <glm/glm.hpp>

#include "../mesh.hpp"
#include "../platform.h"

namespace cmw::elements {

class Line {
    public:
        Line() = default;
        Line(std::vector<glm::vec3> positions): positions(positions) {
            this->mesh.fill_buffers(this->positions);
        }

        ~Line() = default;

        template <typename ...Args>
        void add_points(Args &&...positions) {
            (this->positions.push_back(positions), ...);
            this->mesh.fill_buffers(this->positions);
        }

        void edit_point(std::size_t idx, glm::vec3 position) {
            this->positions[idx] = position;
            this->mesh.fill_buffers(this->positions);
        }

        inline const std::vector<glm::vec3> &get_positions() const { return this->positions; }
        inline const Mesh &get_mesh()                        const { return this->mesh; }

    private:
        std::vector<glm::vec3> positions;
        Mesh mesh;
};

} // namespace cmw::elements
