#include <string>
#include <memory>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cmw.hpp>

struct Vertex {
    union {
        GLfloat coords[3];
        struct {
            GLfloat x, y, z;
        };
    };
    union {
        GLfloat tex_coords[2];
        struct {
            GLfloat s, t;
        };
    };
};

constexpr Vertex vertices[] = {
    { {-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f} },
    { { 0.5f, -0.5f, -0.5f}, {1.0f, 0.0f} },
    { { 0.5f,  0.5f, -0.5f}, {1.0f, 1.0f} },
    { { 0.5f,  0.5f, -0.5f}, {1.0f, 1.0f} },
    { {-0.5f,  0.5f, -0.5f}, {0.0f, 1.0f} },
    { {-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f} },

    { {-0.5f, -0.5f,  0.5f}, {0.0f, 0.0f} },
    { { 0.5f, -0.5f,  0.5f}, {1.0f, 0.0f} },
    { { 0.5f,  0.5f,  0.5f}, {1.0f, 1.0f} },
    { { 0.5f,  0.5f,  0.5f}, {1.0f, 1.0f} },
    { {-0.5f,  0.5f,  0.5f}, {0.0f, 1.0f} },
    { {-0.5f, -0.5f,  0.5f}, {0.0f, 0.0f} },

    { {-0.5f,  0.5f,  0.5f}, {1.0f, 0.0f} },
    { {-0.5f,  0.5f, -0.5f}, {1.0f, 1.0f} },
    { {-0.5f, -0.5f, -0.5f}, {0.0f, 1.0f} },
    { {-0.5f, -0.5f, -0.5f}, {0.0f, 1.0f} },
    { {-0.5f, -0.5f,  0.5f}, {0.0f, 0.0f} },
    { {-0.5f,  0.5f,  0.5f}, {1.0f, 0.0f} },

    { { 0.5f,  0.5f,  0.5f}, {1.0f, 0.0f} },
    { { 0.5f,  0.5f, -0.5f}, {1.0f, 1.0f} },
    { { 0.5f, -0.5f, -0.5f}, {0.0f, 1.0f} },
    { { 0.5f, -0.5f, -0.5f}, {0.0f, 1.0f} },
    { { 0.5f, -0.5f,  0.5f}, {0.0f, 0.0f} },
    { { 0.5f,  0.5f,  0.5f}, {1.0f, 0.0f} },

    { {-0.5f, -0.5f, -0.5f}, {0.0f, 1.0f} },
    { { 0.5f, -0.5f, -0.5f}, {1.0f, 1.0f} },
    { { 0.5f, -0.5f,  0.5f}, {1.0f, 0.0f} },
    { { 0.5f, -0.5f,  0.5f}, {1.0f, 0.0f} },
    { {-0.5f, -0.5f,  0.5f}, {0.0f, 0.0f} },
    { {-0.5f, -0.5f, -0.5f}, {0.0f, 1.0f} },

    { {-0.5f,  0.5f, -0.5f}, {0.0f, 1.0f} },
    { { 0.5f,  0.5f, -0.5f}, {1.0f, 1.0f} },
    { { 0.5f,  0.5f,  0.5f}, {1.0f, 0.0f} },
    { { 0.5f,  0.5f,  0.5f}, {1.0f, 0.0f} },
    { {-0.5f,  0.5f,  0.5f}, {0.0f, 0.0f} },
    { {-0.5f,  0.5f, -0.5f}, {0.0f, 1.0f} },
};

struct CubeParam {
    glm::vec3 pos, rot_axis;
};

constexpr CubeParam cube_params[] = {
    { glm::vec3( 0.0f,  0.0f,   0.0f), glm::vec3(-0.85f, -0.13f, -0.44f) },
    { glm::vec3( 2.0f,  5.0f, -15.0f), glm::vec3(-0.13f, -0.97f, -0.93f) },
    { glm::vec3(-1.5f, -2.2f, - 2.5f), glm::vec3( 0.70f,  0.30f,  0.06f) },
    { glm::vec3(-3.8f, -2.0f, -12.3f), glm::vec3(-0.87f,  0.37f,  0.51f) },
    { glm::vec3( 2.4f, -0.4f, - 3.5f), glm::vec3( 0.46f,  0.99f,  0.27f) },
    { glm::vec3(-1.7f,  3.0f, - 7.5f), glm::vec3(-0.85f,  0.87f, -0.37f) },
    { glm::vec3( 1.3f, -2.0f, - 2.5f), glm::vec3(-0.62f,  0.38f,  0.84f) },
    { glm::vec3( 1.5f,  2.0f, - 2.5f), glm::vec3(-0.29f,  0.51f,  0.76f) },
    { glm::vec3( 1.5f,  0.2f, - 1.5f), glm::vec3(-0.95f,  0.77f,  0.55f) },
    { glm::vec3(-1.3f,  1.0f, - 1.5f), glm::vec3( 0.93f, -0.13f,  0.85f) },
};

#ifdef CMW_SWITCH

extern "C" void __appExit();

extern "C" void __libnx_exception_handler(ThreadExceptionDump *ctx) {
    MemoryInfo mem_info; u32 page_info;
    svcQueryMemory(&mem_info, &page_info, ctx->pc.x);
    CMW_FATAL("%#x exception with pc=%#lx & lr=%#lx\n", ctx->error_desc,
        ctx->pc.x - mem_info.addr, ctx->lr.x - mem_info.addr);
    __appExit();
}

#endif

constexpr int window_w = 1280, window_h = 720;

int main() {
#ifdef CMW_SWITCH
    CMW_TRY_RC_RETURN(romfsInit());
    CMW_SCOPE_GUARD([]() { romfsExit(); });
    appletInitializeGamePlayRecording();
#endif

    auto app = std::make_shared<cmw::Application>(window_w, window_h, "Cemowy");

    CMW_INFO("Starting\n");

    app->get_window().set_vsync(true);
    app->get_window().set_viewport(window_w, window_h);

    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback([](GLenum source, GLenum type, GLuint id, GLenum severity,
            GLsizei length, const GLchar* message, const void *userParam) {
        CMW_INFO("[GL] (t %#x, s %#x): %s\n", type, severity, message );
    }, nullptr);

    CMW_TRACE("Vendor: %s, GL version: %s, GLSL version: %s\n",
        glGetString(GL_VENDOR), glGetString(GL_VERSION), glGetString(GL_SHADING_LANGUAGE_VERSION));

    app->get_window().register_callback<cmw::KeyPressedEvent>([&window = app->get_window()](auto &e) {
#ifdef CMW_SWITCH
        if (e.get_key() == CMW_SWITCH_KEY_PLUS)
#else
        if ((e.get_key() == CMW_KEY_ENTER) && (e.get_mods() & GLFW_MOD_CONTROL))
#endif
            window.set_should_close(true);
    });

#ifdef CMW_SWITCH
    cmw::Font font{PlSharedFontType_Standard};
#else
    cmw::Font font{"fonts/FontStandard.ttf"};
#endif

    app->get_renderer().set_clear_color({0.18f, 0.20f, 0.25f, 1.0f});

    cmw::gl::Texture2d &cube_tex  = app->get_resource_manager().get_texture("textures/191407_1308820425_orig.jpg");
    cmw::gl::Texture2d &bog_tex   = app->get_resource_manager().get_texture("textures/bog.jpg");
    cmw::gl::Texture2d &white_tex = app->get_resource_manager().get_white_texture();

    cmw::gl::ShaderProgram &cube_program = app->get_resource_manager().get_shader("shaders/cube.vert", "shaders/cube.frag");
    cmw::gl::ShaderProgram &mesh_program = app->get_resource_manager().get_shader("shaders/mesh.vert", "shaders/mesh.frag");

    cmw::elements::Line line = {
        {
            {+0.0f, +0.0f, 0.0f},
            {+1280.0f, +720.0f, 0.0f},
        },
        white_tex,
        cmw::colors::Yellow,
        3.0f
    };

    cmw::elements::Point point = {
        {+1000.0f, +100.0f, 0.0f},
        white_tex,
        cmw::colors::Cyan,
        10.0f
    };

    line.add_points(
        glm::vec3(+100.0f, +300.0f, 0.0f),
        glm::vec3(+1280.0f, +0.0f, 0.0f)
    );

    cmw::elements::Triangle triangle = {
        {
            {{+200.0f, +200.0f, 0.0f}, {0.0f, 0.0f}},
            {{+600.0f, +200.0f, 0.0f}, {1.0f, 0.0f}},
            {{+400.0f, +600.0f, 0.0f}, {0.5f, 1.0f}},
        },
        bog_tex,
        cmw::colors::Magenta
    };

    cmw::elements::Rectangle rectangle = {
        {
            {{+ 800.0f, +400.0f, 0.0f}, {0.0f, 0.0f}},
            {{+1000.0f, +400.0f, 0.0f}, {1.0f, 0.0f}},
            {{+1000.0f, +600.0f, 0.0f}, {1.0f, 1.0f}},
            {{+ 800.0f, +600.0f, 0.0f}, {0.0f, 1.0f}},
        },
        bog_tex,
        cmw::colors::Green
    };

    cmw::gl::VertexArray cube_vao;
    cmw::gl::VertexBuffer cube_vbo;
    cube_vbo.set_data(vertices, sizeof(vertices));
    cube_vbo.set_layout({
        cmw::gl::BufferElement::Float3,
        cmw::gl::BufferElement::Float2,
    });

    cube_vao.bind();
    cube_program.bind();
    cube_program.set_value("tex", 0);

    constexpr glm::vec3 cam_pos = glm::vec3(0.0f, 0.0f, 3.0f), cam_front = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::mat4 view_mat  = glm::lookAt(cam_pos, cam_pos + cam_front, glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 proj_mat  = glm::perspective(glm::radians(45.0f), (float)window_w / (float)window_h, 0.1f, 100.0f);
    cube_program.set_value("view_proj", proj_mat * view_mat);

    cmw::OrthographicCamera camera = {0.0f, (float)window_w, 0.0f, (float)window_h};

    cmw::Colorf text_color{0.7f, 0.8f, 0.3f, 1.0f};
    while (!app->get_window().get_should_close()) {
        app->get_renderer().clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        cmw::gl::Texture2d::active(0);
        cube_tex.bind();
        cube_vao.bind();
        cube_program.bind();

        for (std::size_t i = 0; i < 10; ++i) {
            glm::mat4 model = glm::translate(glm::mat4(1.0f), cube_params[i].pos);
            model = glm::rotate(model, (float)glfwGetTime(), cube_params[i].rot_axis);
            cube_program.set_value("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        camera.set_rotation(glm::degrees(app->get_time<float>()));

        app->get_renderer().begin_scene(camera);

        app->get_renderer().submit(line, glm::mat4(1.0f), mesh_program);
        app->get_renderer().submit(point, glm::mat4(1.0f), mesh_program);
        app->get_renderer().submit(triangle,
            glm::rotate(glm::mat4(1.0f), app->get_time<float>(), glm::vec3(0.0f, 0.0f, 1.0f)), mesh_program);
        app->get_renderer().submit(rectangle, glm::mat4(1.0f), mesh_program);

        font.draw_string(app->get_window(), u"123 Hello world\nBazinga é_è $£€", 100.0f, 300.0f, 0.5f, text_color);

        app->get_renderer().end_scene();

        cmw::imgui::begin_frame();

#ifdef CMW_DEBUG
        ImGui::SetNextWindowPos(ImVec2(900, 10), ImGuiCond_Once);
        ImGui::Begin("Debug panel", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
        ImGui::Text("%#.2f fps", ImGui::GetIO().Framerate);
        ImGui::Separator();

        ImGui::ColorEdit3("Clear color", (float *)&app->get_renderer().get_clear_color(), ImGuiColorEditFlags_PickerHueWheel);
        ImGui::ColorEdit3("Text color",  (float *)&text_color,  ImGuiColorEditFlags_PickerHueWheel);

        ImGui::End();
#endif

        cmw::imgui::end_frame();
        app->get_window().update();
    }

    CMW_INFO("Exiting\n");

    return 0;
}
