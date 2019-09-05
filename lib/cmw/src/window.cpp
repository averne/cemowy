#include <string>
#include <stdexcept>
#include <utility>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <cmw/platform.h>

#include <cmw/window.hpp>

namespace cmw {

Window::Window(int w, int h, const std::string &name, int x, int y, GLboolean resizable, GlVersion ver) {
    CMW_TRACE("Creating window object\n");
    glfwInit();
    set_gl_version(ver);
    hint(std::pair{GLFW_RESIZABLE, resizable});

    CMW_TRY_THROW((this->window = glfwCreateWindow(w, h, name.c_str(), nullptr, nullptr)),
        std::runtime_error("Could not create window object"));
    make_ctx_current();
    set_pos(x, y);
    glfwSetWindowUserPointer(get_window(), this);

    CMW_TRY_THROW(gladLoadGLLoader((GLADloadproc)glfwGetProcAddress),
        std::runtime_error("Failed to initialize glad"));

    this->input_manager.set_window(get_window());
}

} // namespace cmw
