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

    CMW_TRY_RC_RETURN(cmw::log::initialize());
    CMW_SCOPE_GUARD([]() { cmw::log::finalize(); });

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

    window->get_input_manager()->register_callback<cmw::KeyPressedEvent>([&window](auto &e) {
#ifdef CMW_SWITCH
        if (e.get_key() == CMW_SWITCH_KEY_PLUS)
#else
        if ((e.get_key() == CMW_KEY_ENTER) && (e.get_mods() & GLFW_MOD_CONTROL))
#endif
            window->set_should_close(true);
    });

    cmw::imgui::initialize(window);
    CMW_SCOPE_GUARD([]() { cmw::imgui::finalize(); });

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

    cmw::Colorf clear_color{0.18f, 0.20f, 0.25f, 1.0f};
    while (!window->get_should_close()) {
        glClearColor(clear_color.r, clear_color.g, clear_color.b, clear_color.a);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glDrawArrays(GL_TRIANGLES, 0, 3);

        cmw::imgui::begin_frame();

        ImGui::Begin("Debug panel", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
        ImGui::SetWindowPos(ImVec2(900, 10), ImGuiCond_Once);
        ImGui::ColorPicker3("Clear color", (float *)&clear_color,
            ImGuiColorEditFlags_PickerHueWheel | ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_DisplayHex);
        ImGui::End();

        cmw::imgui::end_frame();
        window->update();
    }

    CMW_INFO("Exiting\n");

    return 0;
}
