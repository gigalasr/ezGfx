#include "ez/Core/UIApp.hpp"

#include <GLFW/glfw3.h>
#include <glad/gl.h>

#include <glm/glm.hpp>

#include "ez/Core/Base.hpp"
#include "ez/Core/Profiling.hpp"

namespace ez {
    UIApp* ez::UIApp::s_instance = nullptr;

    void UIApp::on_framebuffer_size_changed(GLFWwindow* window, int width, int height) {
        if (width > 0 && height > 0) {
            UIApp::get().graphics->set_render_size(width, height);
            UIApp::get().width = width;
            UIApp::get().height = height;
        }
    }

    static void error_callback(int, const char* err_str) { EZ_CORE_DEBUG("GLFW: ", err_str); }

    UIApp::UIApp(int width, int height, const std::string& title) : width(width), height(height) {
        s_instance = this;

        ez::Logger::init();
        EZ_CORE_LOG("ezUI Version v", ez::VERSION_MAJOR, ".", ez::VERSION_MINOR, ".", ez::VERSION_PATCH);
        EZ_PROFILE_BEGIN_SESSION("Startup");

        glfwInit();
        glfwSetErrorCallback(error_callback);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_SAMPLES, 4);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef EZ_BUILD_DEBUG_MODE
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
#endif

        m_window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
        EZ_CORE_ASSERT(m_window != nullptr, "Failed to create window");

        glfwMakeContextCurrent(m_window);
        glfwSetFramebufferSizeCallback(m_window, UIApp::on_framebuffer_size_changed);
        glfwSetWindowUserPointer(m_window, this);

        device = RenderAPI::create(ez::API::OPENGL);
        graphics = Renderer::create(device, width, height);

        EZ_PROFILE_END_SESSION();
    }

    UIApp::~UIApp() {
        EZ_PROFILE_BEGIN_SESSION("shutdown");

        glfwTerminate();

        EZ_PROFILE_END_SESSION();
    }

    void UIApp::run() {
        EZ_PROFILE_BEGIN_SESSION("Runtime");

        while (!glfwWindowShouldClose(m_window)) {
            // glfwWaitEvents();
            glfwPollEvents();

            static double lastFrameTime = 0;
            double currentFrameTime = glfwGetTime();
            double delta_time = currentFrameTime - lastFrameTime;

            std::string frameRate = "FPS: ";
            frameRate += std::to_string((1 / delta_time));
            glfwSetWindowTitle(m_window, frameRate.c_str());

            graphics->begin_frame();
              render(currentFrameTime, delta_time);
            graphics->end_frame();

            glfwSwapBuffers(m_window);
            lastFrameTime = currentFrameTime;
        }

        EZ_PROFILE_END_SESSION();
    }

    UIApp& UIApp::get() { return *s_instance; }
}  // namespace ez