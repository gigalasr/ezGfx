#include "Renderer.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "ez/Core/Base.hpp"
#include "ez/Core/Profiling.hpp"
#include "ez/Core/StrongHandle.hpp"
#include "ez/Graphics/API/OpenGL/GL_BuiltinShader.hpp"
#include "ez/Graphics/API/RenderAPI.hpp"
#include "ez/Graphics/Renderer/Font.hpp"

namespace ez {
    constexpr int MAX_QUADS = 20000;

    struct QuadData {
        float transform[10];
        uint32_t data_2[2];
    };

    Ref<Renderer> Renderer::create(Ref<RenderAPI> device, uint32_t width, uint32_t height) { 
        struct make_shared_enabler : public Renderer { 
            make_shared_enabler(Ref<RenderAPI> device, uint32_t width, uint32_t height) : Renderer(device, width, height) {} 
        };
        return create_ref<make_shared_enabler>(device, width, height);     
    }

    Renderer::Renderer(Ref<RenderAPI> device, uint32_t width, uint32_t height) 
        : m_device(device), m_render_width(width), m_render_height(height) {
        EZ_PROFILE_FUNCTION();
        EZ_CORE_DEBUG_ALLOC("Initializing Renderer");

        // Load Shader
        m_quad_shader = m_device->create_shader({{Shader::Type::VERTEX, ez::GL_QUAD_VERTEX_SHADER}, {Shader::Type::FRAGMENT, ez::GL_QUAD_FRAGMENT_SHADER}});
        m_quad_shader->bind();

        m_quad_storage = m_device->create_stream_storage(MAX_QUADS, sizeof(QuadData));

        m_brushes = m_device->create_texture_array(2, 2, Format::RGBA, Filter::LINEAR);
        m_brushes->bind_to_slot(1);
        m_quad_shader->set("uBrushTex", 1);
        EZ_CORE_DEBUG("Max Texture Array Layers: ", m_brushes->get_max_layers());

        // m_Font = CreateRef<Font>("C:\\Windows\\Fonts\\segoeui.ttf",
        // m_device); m_Font->get_texture()->bind_to_slot(2);

        // Setup default state
        m_device->set_clear_color(0.15f, 0.15f, 0.15f, 1.0f);
        set_render_size(width, height);
    }

    Renderer::~Renderer() {
        EZ_PROFILE_FUNCTION();
        EZ_CORE_DEBUG_DEALLOC("Shutting down Renderer");
    }

    void Renderer::set_render_size(uint32_t width, uint32_t height) {
        EZ_PROFILE_FUNCTION();

        m_render_width = width;
        m_render_height = height;

        float fovy = glm::radians(45.0f);
        float aspect = (float)m_render_width / (float)m_render_height;
        float depthZ = (-1.0f * m_render_height) / (2.0f * tanf(fovy / 2.0f));
        glm::mat4 proj = glm::perspective(fovy, aspect, 1.0f, m_render_height * 2.0f);

        m_quad_shader->set("uProj", proj);
        m_quad_shader->set("uOffset", depthZ);
        set_view_matrix();

        m_device->set_viewport(0, 0, width, height);
    }

    void Renderer::set_view_matrix() {
        EZ_PROFILE_FUNCTION();
        glm::mat4 view(1.0f);
        view = glm::translate(view, glm::vec3(-0.5f * m_render_width, 0.5f * m_render_height, 0.0f));
        view = glm::scale(view, glm::vec3(1.0f, -1.0f, 1.0f));
        m_quad_shader->set("uView", view);
    }

    void Renderer::begin_frame() {
        EZ_PROFILE_FUNCTION();
        m_quad_storage->begin_frame();
    }

    void Renderer::end_frame() {
        EZ_PROFILE_FUNCTION();
        m_device->clear();
        m_device->draw_arrays(DrawMode::TRIANGLES, m_quad_storage->count() * 6, m_quad_storage->offset() * 6);
        m_quad_storage->end_frame();
    }

    Brush Renderer::create_solid_color_brush(Color color) {
        uint8_t colorData[]{
            color.r, color.g, color.b, color.a, color.r, color.g, color.b, color.a, color.r, color.g, color.b, color.a, color.r, color.g, color.b, color.a,
        };

        return Brush((int32_t)m_brushes->push_back(colorData));
    }

    Brush Renderer::create_gradient_brush(Color color1, Color color2) {
        uint8_t colorData[]{
            color1.r, color1.g, color1.b, color1.a, color1.r, color1.g, color1.b, color1.a,
            color2.r, color2.g, color2.b, color2.a, color2.r, color2.g, color2.b, color2.a,
        };

        return Brush((int32_t)m_brushes->push_back(colorData));
    }

    void Renderer::draw_rect(Brush brush, const glm::vec3& position, const glm::vec2& size, const glm::vec3& axies, float angle) {
        EZ_PROFILE_FUNCTION();

        QuadData data{{position.x, position.y, position.z, size.x, size.y, axies.x, axies.y, axies.z, angle, 0}, {(uint32_t)brush.get(), 0}};

        m_quad_storage->push_back(&data);
    }

    void Renderer::draw_rect(Brush brush, const glm::vec3& position, const glm::vec2& size) {
        EZ_PROFILE_FUNCTION();

        QuadData data{{position.x, position.y, position.z, size.x, size.y, 0, 0, 0, 1, 0}, {(uint32_t)brush.get(), 0}};

        m_quad_storage->push_back(&data);
    }
}  // namespace ez