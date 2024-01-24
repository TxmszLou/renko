#pragma once

#include "vertexBuffer.h"

class VertexBufferLayout;

class VertexArray
{
public:
    VertexArray();
    ~VertexArray();

    void Init();

    void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);

    void Bind() const;
    void Unbind() const;

private:
    unsigned int m_RendererID;
};