#pragma once

/* A class to abstract the id of vertex buffer, bind, unbind and destroy */
class IndexBuffer
{
    private:
        // my buffer id
        uint32_t m_RendererID;
        // how many indices are there
        uint32_t m_Count;
    public:
        IndexBuffer();
        ~IndexBuffer();

        void Init(const uint32_t* data, uint32_t count);

        void Bind() const;
        void Unbind() const;

        inline uint32_t GetCount() const { return m_Count; }
};