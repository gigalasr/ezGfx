#include "ez/ez.hpp"

#include <iostream>
#include <glm/gtc/matrix_transform.hpp>

class App : public ez::UIApp {
    ez::Brush defaultBrush;
	ez::Brush defaultBrush2;
	ez::Brush gradientBrush;


public:
    App() : ez::UIApp(800, 600, "Sandbox") {
		defaultBrush = ez::Renderer2D::create_solid_color_brush(ez::Color(255));
		defaultBrush2 = ez::Renderer2D::create_solid_color_brush(ez::Color (0));
		gradientBrush = ez::Renderer2D::create_gradient_brush(ez::Color(0.98f, 0.18f, 0.63f, 1.0f),
                                                               ez::Color(0.94f, 0.24f, 0.3f, 1.0f));        
    }

    void render(double time, double delta_time) override {
        ez::Renderer2D::draw_rect(gradientBrush, glm::vec3(0, 0, 0), glm::vec3(width, height, 0),
                                       glm::vec3(0, 0, 0));

	    for (int x = 0; x < 16; x++) {
		    for (int y = 0; y < 16; y++) {
                ez::Renderer2D::draw_rect(defaultBrush,
                                       glm::vec3(x * 65, y * 65, sin(4 * time + x + y) * 10),
                                       glm::vec2(60, 60), glm::vec3(0));
		    }
    	}
    }
};

int main() {
    auto app = new App();
    app->run();
   
    EZ_LOG("App Exiting...");
}
