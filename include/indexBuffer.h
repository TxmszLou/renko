#pragma once

/* A class to abstract the id of vertex buffer, bind, unbind and destroy */
class IndexBuffer
{
    private:
        // my buffer id
        unsigned int m_RendererID;
        // how many indices are there
        unsigned int m_Count;
    public:
        IndexBuffer(const unsigned int* data, unsigned int size);
        ~IndexBuffer();

        void Bind() const;
        void Unbind() const;

        inline unsigned int GetCount() const { return m_Count; }
};