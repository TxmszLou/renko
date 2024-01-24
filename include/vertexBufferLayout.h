#pragma once

#include <vector>

#include <glad/glad.h>
#include "renderer.h"

struct VertexBufferElement
{
    uint32_t type;
    uint32_t count;
    unsigned char normalized;

    static uint32_t GetSizeOfType(uint32_t type)
    {
        switch(type)
        {
            case GL_FLOAT:          return 4;
            case GL_UNSIGNED_INT:   return 4;
            case GL_UNSIGNED_BYTE:  return 1;
        }
        ASSERT(false);
        return 0;
    }
};

class VertexBufferLayout
{
public:
    VertexBufferLayout()
        : m_Stride(0) {}
    template<typename T>
    void Push(uint32_t count)
    {
        // should not be called
        // static_assert(false);
    }


    inline const std::vector<VertexBufferElement>& GetElements() const { return m_Elements; }
    inline uint32_t GetStride() const { return m_Stride; }
    inline void Reset() { m_Elements.clear(); m_Stride = 0; }

private:
    std::vector<VertexBufferElement> m_Elements;
    uint32_t m_Stride;
};