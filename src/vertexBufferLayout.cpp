#include "vertexBufferLayout.h"
#include "glad/glad.h"

// push different types of elements
template<>
void VertexBufferLayout::Push<float>(uint32_t count) {
    m_Elements.push_back({ GL_FLOAT, count, GL_FALSE });
    m_Stride += count * VertexBufferElement::GetSizeOfType(GL_FLOAT);
}

template<>
void VertexBufferLayout::Push<uint32_t>(uint32_t count) {
    m_Elements.push_back({ GL_UNSIGNED_INT, count, GL_FALSE });
    m_Stride += count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_INT);
}

template<>
void VertexBufferLayout::Push<unsigned char>(uint32_t count) {
    m_Elements.push_back({ GL_UNSIGNED_BYTE, count, GL_TRUE });
    m_Stride += count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_BYTE);
}

template<>
void VertexBufferLayout::Push<glm::vec2>(uint32_t count) {
    m_Elements.push_back({ GL_FLOAT, count * 2, GL_TRUE });
    m_Stride += count * 2 * VertexBufferElement::GetSizeOfType(GL_FLOAT);
}

template<>
void VertexBufferLayout::Push<glm::vec4>(uint32_t count) {
    m_Elements.push_back({ GL_FLOAT, count * 4, GL_TRUE });
    m_Stride += count * 4 * VertexBufferElement::GetSizeOfType(GL_FLOAT);
}