#pragma once

#include <cstdint>

namespace ez {
    class StreamStorage {
    public:
        virtual ~StreamStorage() = default;
        virtual void push_back(void* data) = 0;

        virtual uint32_t count() = 0;
        virtual uint32_t offset() = 0;

        virtual void begin_frame() = 0;
        virtual void end_frame() = 0;
    };
}