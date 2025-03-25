#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

#include "ez/ez.hpp"

class App : public ez::UIApp {
    ez::Brush defaultBrush;
    ez::Brush defaultBrush2;
    ez::Brush gradientBrush;

   public:
    App() : ez::UIApp(800, 600, "Sandbox") {
        defaultBrush = graphics->create_solid_color_brush(ez::Color(255));
        defaultBrush2 = graphics->create_solid_color_brush(ez::Color(0));
        gradientBrush = graphics->create_gradient_brush(ez::Color(0.98f, 0.18f, 0.63f, 1.0f), ez::Color(0.94f, 0.24f, 0.3f, 1.0f));
    }

    void render(double time, double delta_time) override {
        graphics->draw_rect(gradientBrush, glm::vec3(0, 0, 0), glm::vec3(width, height, 0), glm::vec3(0, 0, 0), 0);

        for (int x = 0; x < 16; x++) {
            for (int y = 0; y < 16; y++) {
                graphics->draw_rect(defaultBrush, glm::vec3(30 + x * 35, 30 + y * 35, 20 * sin((x + y*0.5 + time))), glm::vec2(30, 30), glm::vec3(0,0,1), 0);
            }
        }
    }
};

int main() {
    auto app = new App();
    app->run();

    EZ_LOG("App Exiting...");
}
