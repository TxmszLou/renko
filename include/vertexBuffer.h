#pragma once

#include <cstdint>

/* A class to abstract the id of vertex buffer, bind, unbind and destroy */
class VertexBuffer
{
    private:
        // my buffer id
        uint32_t m_RendererID;
    public:
        VertexBuffer();
        ~VertexBuffer();

        // init static buffer
        void Init(const void* data, uint32_t size);
        // init dynamic buffer
        void Init(uint32_t size);

        void Bind() const;
        void Unbind() const;
};