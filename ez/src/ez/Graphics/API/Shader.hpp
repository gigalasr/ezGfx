#pragma once

#include <glm/glm.hpp>
#include <string>

namespace ez {
    class Shader {
       public:
        enum class Type { VERTEX = 0, FRAGMENT };

        virtual ~Shader() = default;
        virtual void bind() = 0;
        virtual void set(const std::string& location, const glm::mat4& matrix) = 0;
        virtual void set(const std::string& location, float value) = 0;
        virtual void set(const std::string& location, int value) = 0;
    };
}  // namespace ez