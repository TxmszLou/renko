#include "renderer.h"
#include "indexBuffer.h"

IndexBuffer::IndexBuffer()
{
    ASSERT(sizeof(uint32_t) == sizeof(GLuint));
}

void IndexBuffer::Init(const uint32_t* data, uint32_t count)
{
    m_Count = count;
    GLCall(glGenBuffers(1, &m_RendererID));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID)); // select the buffer, tells the GPU buffer is just an array
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), data, GL_STATIC_DRAW)); // copy data to vertex buffer
}

IndexBuffer::~IndexBuffer()
{
    GLCall(glDeleteBuffers(1, &m_RendererID));
}

void IndexBuffer::Bind() const
{
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
}

void IndexBuffer::Unbind() const
{
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}
