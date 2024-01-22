#pragma once

#include "tests/test.h"

#include "vertexArray.h"
#include "vertexBuffer.h"
#include "vertexBufferLayout.h"
#include "texture.h"

#include <memory>

namespace test {
    // making a quad with solid color
    class TestTexQuad : public Test {
    public:
        TestTexQuad();
        ~TestTexQuad();

        void OnUpdate(float deltaTime) override;
        void OnRender() override;
        void OnImGuiRender() override;

    private:
        // translation matrix
        glm::vec3 m_Translation;

        std::unique_ptr<VertexArray> m_VAO;
        std::unique_ptr<VertexBuffer> m_VB;
        std::unique_ptr<IndexBuffer> m_IBO;
        std::unique_ptr<Shader>      m_Shader;
        std::vector<std::unique_ptr<Texture>> m_Textures;

        glm::mat4 m_Proj, m_View;
    };
}