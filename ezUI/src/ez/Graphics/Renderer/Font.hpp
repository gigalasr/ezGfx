#pragma once
#include <memory>
#include <string>

#include "ez/Core/Base.hpp"
#include "ez/Graphics/API/Enums.hpp"
#include "ez/Graphics/API/RenderAPI.hpp"

namespace ez {
    class Font {
       public:
        Font(const std::string& file, const Ref<RenderAPI>& texture);
        const Ref<TextureArray>& get_texture();

       private:
        Ref<TextureArray> m_texture;

        int m_ascent;
        int m_descent;
        int m_line_gap;
    };
}  // namespace ez