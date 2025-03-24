#pragma once
#include <functional>
#include <string>

#include "ez/Core/Base.hpp"

typedef struct GLFWwindow GLFWwindow;

namespace ez {
    class UIApp {
       public:
        /**
         * Creates a new UIApp with the provided specification
         * @param spec Specification
         */
        UIApp(int width, int height, const std::string& title);

        /**
         * Destructor
         */
        ~UIApp();

        /**
         * Gets the instance of the UIApp
         * @return current instance of the App
         */
        static UIApp& get();

        /**
         * Runs the app
         */
        void run();

        virtual void render(double time, double delta_time) = 0;

       protected:
        int width;
        int height;

       private:
        /**
         * The framebuffer size changed callback get called once the window has
         * been resized
         * @param window pointer to the window that has been resized
         * @param width new window width
         * @param height new window height
         */
        static void on_framebuffer_size_changed(GLFWwindow* window, int width, int height);

        static UIApp* s_instance;
        GLFWwindow* m_window;
    };
}  // namespace ez