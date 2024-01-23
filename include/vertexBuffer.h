#pragma once

/* A class to abstract the id of vertex buffer, bind, unbind and destroy */
class VertexBuffer
{
    private:
        // my buffer id
        unsigned int m_RendererID;
    public:
        // static buffer
        VertexBuffer(const void* data, unsigned int size);
        // dynamic buffer
        VertexBuffer(unsigned int size);
        ~VertexBuffer();

        void Bind() const;
        void Unbind() const;
};