#pragma once

/* A class to abstract the id of vertex buffer, bind, unbind and destroy */
class VertexBuffer
{
    private:
        // my buffer id
        unsigned int m_RendererID;
    public:
        VertexBuffer();
        ~VertexBuffer();

        // init static buffer
        void Init(const void* data, unsigned int size);
        // init dynamic buffer
        void Init(unsigned int size);

        void Bind() const;
        void Unbind() const;
};