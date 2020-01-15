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

#include <string>
#include <memory>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cmw.hpp>

#include "data.hpp"

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

    app->get_window().register_callback<cmw::input::KeyPressedEvent>([&window = app->get_window()](const auto &e) {
#ifdef CMW_SWITCH
        if (e.get_key() == cmw::input::KeySwitchPlus)
#else
        if ((e.get_key() == cmw::input::KeyEnter) && e.has_mod(cmw::input::ModControl))
#endif
            window.set_should_close(true);
    });

#ifdef CMW_SWITCH
    app->get_resource_manager().load_font(PlSharedFontType_Standard);
    app->get_resource_manager().load_font(PlSharedFontType_NintendoExt);
#else
    app->get_resource_manager().load_font("fonts/FontStandard.ttf");
    app->get_resource_manager().load_font("fonts/FontNintendoExtended.ttf");
#endif
    auto *comic_sans = app->get_resource_manager().load_font("fonts/comic.ttf");

    app->get_renderer().set_clear_color({0.18f, 0.20f, 0.25f, 1.0f});

    cmw::gl::Texture2d &cube_tex  = app->get_resource_manager().get_texture("textures/rectangle.jpg");
    cmw::gl::Texture2d &bog_tex   = app->get_resource_manager().get_texture("textures/triangle.jpg");
    cmw::gl::Texture2d &white_tex = app->get_resource_manager().get_white_texture();

    cmw::gl::ShaderProgram &cube_program = app->get_resource_manager().get_shader("shaders/cube.vert", "shaders/cube.frag");

    cmw::shapes::Line line = {
        {
            {+   0.0f, +  0.0f, +0.0f},
            {+1280.0f, +720.0f, +0.0f},
        },
        white_tex,
        cmw::colors::Yellow,
        3.0f
    };

    cmw::shapes::Point point = {
        {+100.0f, +600.0f, +0.0f},
        white_tex,
        cmw::colors::Cyan,
        10.0f
    };

    line.add_points(
        glm::vec3(+ 100.0f, +300.0f, +0.0f),
        glm::vec3(+1280.0f, +  0.0f, +0.0f)
    );

    auto triangle = cmw::AnimatedObject<cmw::shapes::Triangle>(
        [](auto elapsed) -> glm::mat4 {
            return glm::rotate(glm::mat4(1.0f), (float)(elapsed.count() / 1000.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        },
        std::vector<cmw::Mesh::Vertex>{
            {{+200.0f, +200.0f, +1.0f}, {0.0f, 0.0f}},
            {{+600.0f, +200.0f, +1.0f}, {1.0f, 0.0f}},
            {{+400.0f, +600.0f, +1.0f}, {0.5f, 1.0f}},
        },
        bog_tex,
        cmw::colors::Magenta
    );

    auto rectangle = cmw::AnimatedObject<cmw::shapes::Rectangle>(
        [](auto elapsed) -> glm::mat4 {
            float scale = glm::sin((float)(elapsed.count() / 1000.0f)) / 2.0f + 1.0f;
            return glm::scale(glm::mat4(1.0f), glm::vec3(scale));
        },
        std::vector<cmw::Mesh::Vertex>{
            {{+ 800.0f, +400.0f, -1.0f}, {0.0f, 0.0f}},
            {{+1000.0f, +400.0f, -1.0f}, {1.0f, 0.0f}},
            {{+1000.0f, +600.0f, -1.0f}, {1.0f, 1.0f}},
            {{+ 800.0f, +600.0f, -1.0f}, {0.0f, 1.0f}},
        },
        white_tex,
        cmw::colors::Green
    );

    cmw::shapes::Circle circle = {
        {+ 800.0f, +200.0f, +0.0f},
        100.0f,
        cube_tex,
        cmw::colors::Blue
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

    cmw::OrthographicCamera camera = {0.0f, (float)window_w, 0.0f, (float)window_h, -10.0f, 10.0f};

    cmw::widgets::Scene my_scene({0, 0}, {window_w, window_h});
    cmw::widgets::Button button(&my_scene, {{10, 10}, {100, 100}}, "test");

    app->get_window().get_input_manager().register_callback<cmw::input::KeyPressedEvent>([&camera](const auto &e) {
#ifdef CMW_SWITCH
        if (e.get_key() == cmw::input::KeySwitchDleft)
            camera.rotate(1.0f);
        if (e.get_key() == cmw::input::KeySwitchDright)
            camera.rotate(-1.0f);
#else
        if (e.get_key() == cmw::input::KeyLeft)
            camera.rotate(1.0f);
        if (e.get_key() == cmw::input::KeyRight)
            camera.rotate(-1.0f);
#endif
    });

    auto anim = cmw::TimedAnimation<void>(1000ms, [](auto elapsed) -> void {
        CMW_TRACE("Animation: %ldms elapsed\n", elapsed.count());
    });

    float t, dt, last_time = app->get_time<float>();
    cmw::Colorf text_color{cmw::colors::Red};
    while (!app->get_window().get_should_close()) {
        app->get_renderer().clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (anim)
            anim.update();

        t = app->get_time<float>();
        dt = t - last_time, last_time = t;

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

        app->get_renderer().begin(camera, dt, GL_POINTS);
        app->get_renderer().submit(point, glm::mat4(1.0f));
        app->get_renderer().end();

        app->get_renderer().begin(camera, dt, GL_LINES);
        app->get_renderer().submit(line, glm::mat4(1.0f));
        app->get_renderer().end();

        app->get_renderer().begin(camera, dt);
        app->get_renderer().submit(triangle);
        app->get_renderer().submit(rectangle);
        app->get_renderer().submit(circle, glm::mat4(1.0f));
        app->get_renderer().draw_string(u"\ue000\ue044\ue122\n", {300.0f, 550.0f, 1.0f}, 0.5f, text_color);
        app->get_renderer().draw_string(comic_sans, u"Comic \ue000 sans \ue044 best \ue122 sans", {150.0f, 400.0f, 1.0f}, 0.2f, text_color);
        app->get_renderer().draw_string(u"\ue121 123 Hello world\nBazinga é_è $£€", {100.0f, 300.0f, 1.0f}, 0.5f, text_color);

        app->get_renderer().submit(my_scene, glm::mat4(1.0f));

        app->get_renderer().end();

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
