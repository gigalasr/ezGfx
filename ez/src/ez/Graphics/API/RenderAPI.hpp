#pragma once
#include <glm/glm.hpp>
#include <initializer_list>
#include <memory>
#include <string>

#include "ez/Core/Base.hpp"
#include "ez/Graphics/API/Enums.hpp"
#include "ez/Graphics/API/Shader.hpp"
#include "ez/Graphics/API/StreamStorage.hpp"
#include "ez/Graphics/API/TextureArray.hpp"

namespace ez {
    enum class API { NONE = 0, OPENGL };

    class RenderAPI {
       public:
        virtual ~RenderAPI() = default;

        static Ref<RenderAPI> create(ez::API api);

        virtual Ref<Shader> create_shader(const std::initializer_list<std::pair<Shader::Type, const std::string&>>& shaders) = 0;
        virtual Ref<StreamStorage> create_stream_storage(uint32_t count, uint32_t stride) = 0;
        virtual Ref<TextureArray> create_texture_array(uint32_t width, uint32_t height, Format format, Filter filter) = 0;

        virtual void clear() = 0;
        virtual void set_clear_color(float r, float g, float b, float a) = 0;
        virtual void set_viewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;

        virtual void draw_arrays(DrawMode mode, uint32_t count, uint32_t offset) = 0;
    };
}  // namespace ez