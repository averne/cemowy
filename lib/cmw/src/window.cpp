#include <stdexcept>
#include <utility>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <cmw/platform.h>

#include <cmw/window.hpp>

namespace cmw {

Window::Window(int w, int h, const char *name, int x, int y, GLboolean resizable, GlVersion ver) {
    CMW_TRACE("Creating window object\n");
    set_gl_version(ver);
    hint(std::pair{GLFW_RESIZABLE, resizable});
    CMW_TRY_THROW((this->window = glfwCreateWindow(w, h, name, nullptr, nullptr)),
        std::runtime_error("Could not create window object"));
    make_ctx_current();
    set_pos(x, y);
    set_vsync(true);
    glfwSetWindowUserPointer(get_window(), this);
    this->input_manager = std::make_unique<InputManager>(get_window());
}

} // namespace cmw
