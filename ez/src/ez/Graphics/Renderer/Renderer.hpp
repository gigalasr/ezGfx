#pragma once
#include <glm/glm.hpp>

#include "ez/Core/Base.hpp"
#include "ez/Core/StrongHandle.hpp"
#include "ez/Graphics/API/Enums.hpp"
#include "ez/Graphics/API/RenderAPI.hpp"
#include "ez/Graphics/Renderer/Color.hpp"
#include "ez/Graphics/Renderer/Font.hpp"

namespace ez {
    EZ_MAKE_STRONG_HANDLE(Brush);
    EZ_MAKE_STRONG_HANDLE(Sprite);

    class Renderer {
       public:
        /**
         * Initializes the Renderer2D
         * @param device the graphics device used to render
         * @param width width of the render target
         * @param height height of the render target
         */
        static Ref<Renderer> create(Ref<RenderAPI> device, uint32_t width, uint32_t height);

        /**
         * Shuts the Renderer2D down and frees resources
         */
        ~Renderer();

        /**
         * Set the current view matrix
         * @param matrix new view matrix
         */
        void set_view_matrix();

        /**
         * Set the size of the render target
         * @param width new width
         * @param height new height
         */
        void set_render_size(uint32_t width, uint32_t height);

        /**
         * Starts a new frame
         */
        void begin_frame();

        /**
         * Ends and draws the current frame
         */
        void end_frame();

        /**
         * Creates a new single colored brush
         * @param color color of the brush
         * @return new single colored Brush
         */
        Brush create_solid_color_brush(Color color);

        /**
         * Creates a new gradient brush between two colors
         * @param color1 first Color
         * @param color2 second Color
         * @return new gradient brush
         */
        Brush create_gradient_brush(Color color1, Color color2);

        /**
         * Draws a rotated rectangle
         * @param brush brush to use
         * @param position position of the top left corner
         * @param size width and height
         * @param rotation rotation around each axis
         */
        void draw_rect(Brush brush, const glm::vec3& position, const glm::vec2& size, const glm::vec3& rotation);

        /**
         * Draws a rectangle
         * @param brush brush to use
         * @param position position of the top left corner
         * @param size width and height
         */
        void draw_rect(Brush brush, const glm::vec3& position, const glm::vec2& size);

       private:
        /**
         * Initializes the Renderer2D
         * @param device the graphics device used to render
         * @param width width of the render target
         * @param height height of the render target
         */
        Renderer(Ref<RenderAPI> device, uint32_t width, uint32_t height);

        Ref<RenderAPI> m_device;
        Ref<TextureArray> m_brushes;
        Ref<StreamStorage> m_quad_storage;
        Ref<Shader> m_quad_shader;
        Ref<Font> m_font;
        uint32_t m_render_width;
        uint32_t m_render_height;
    };  // namespace Renderer2D
}  // namespace ez