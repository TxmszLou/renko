#pragma once

/* A class to abstract the id of vertex buffer, bind, unbind and destroy */
class VertexBuffer
{
    private:
        // my buffer id
        unsigned int m_RendererID;
    public:
        VertexBuffer(const void* data, unsigned int size);
        ~VertexBuffer();

        void Bind() const;
        void Unbind() const;
};