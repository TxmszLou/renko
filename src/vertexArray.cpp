#include "vertexBufferLayout.h"
#include "vertexArray.h"
#include "renderer.h"

VertexArray::VertexArray()
{
}

VertexArray::~VertexArray()
{
    GLCall(glDeleteVertexArrays(1, &m_RendererID));
}

void VertexArray::Init()
{
    GLCall(glGenVertexArrays(1, &m_RendererID));
}

void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout)
{
    // bind this vertex array
    Bind();
    // bind the vertex buffer, associating to this vertex array
    vb.Bind();
    
    intptr_t offset = 0;
    const auto& elements = layout.GetElements();
    for (unsigned int i = 0; i < elements.size(); i++)
    {
        const auto& element = elements[i];
        GLCall(glEnableVertexAttribArray(i));
        // set vertex attribute
        // this will bind the vertex array buffer to the currently bound vertex array object
        GLCall(glVertexAttribPointer(i, element.count, element.type,
            element.normalized, layout.GetStride(), (void*) offset));
        
        offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
    }
}

void VertexArray::Bind() const {
    GLCall(glBindVertexArray(m_RendererID));
}

void VertexArray::Unbind() const {
    GLCall(glBindVertexArray(0));
}