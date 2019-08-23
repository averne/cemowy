#include <string>
#include <memory>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cmw.hpp>

struct Vertex {
    glm::vec3 position;
    glm::vec3 color;
};

Vertex vertices[] = {
    {{-0.5f, -0.5f, 1.0f}, {1.0f, 0.0f, 0.0f}},
    {{+0.5f, -0.5f, 1.0f}, {0.0f, 1.0f, 0.0f}},
    {{+0.0f, +0.5f, 1.0f}, {0.0f, 0.0f, 1.0f}},
};

constexpr int window_w = 1280, window_h = 720;

int main() {
#ifdef CMW_SWITCH
    CMW_TRY_RC_RETURN(romfsInit());
    CMW_SCOPE_GUARD([]() { romfsExit(); });
#endif

    CMW_TRY_RC_RETURN(cmw::Logger::initialize());
    CMW_SCOPE_GUARD([]() { cmw::Logger::finalize(); });

    glfwInit();
    CMW_SCOPE_GUARD([]() { glfwTerminate(); });

    CMW_INFO("Starting\n");

    auto window = std::make_shared<cmw::Window>(window_w, window_h, "Cemowy");
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize glad" << std::endl;
        return -1;
    }

    window->set_vsync(true);
    window->set_viewport(window_w, window_h);

    cmw::ShaderProgram program = {
        cmw::VertexShader  {"shaders/triangle.vert"},
        cmw::FragmentShader{"shaders/triangle.frag"}
    };

    cmw::VertexArray vao;
    cmw::VertexBuffer vbo;
    vbo.set_data(vertices, sizeof(vertices));
    vbo.set_layout({
        cmw::BufferElement::Float3,
        cmw::BufferElement::Float3,
    });

    vao.bind();
    program.bind();

    while (!window->get_should_close()) {
        glClearColor(0.18f, 0.20f, 0.25f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glDrawArrays(GL_TRIANGLES, 0, 3);

        window->update();
    }

    CMW_INFO("Exiting\n");

    return 0;
}
