#pragma once

#include "tests/test.h"

#include "vertexArray.h"
#include "vertexBuffer.h"
#include "vertexBufferLayout.h"
#include "texture.h"

#include <memory>

namespace test {
    // making a quad with solid color
    class TestQuadSolid : public Test {
    public:
        TestQuadSolid();
        ~TestQuadSolid();

        void OnUpdate(float deltaTime) override;
        void OnRender() override;
        void OnImGuiRender() override;

    private:
        // translation matrix
        glm::vec3 m_TranslationA;
        glm::vec3 m_TranslationB;

        std::unique_ptr<VertexArray> m_VAO;
        std::unique_ptr<IndexBuffer> m_IBO;
        std::unique_ptr<Shader>      m_Shader;

        glm::mat4 m_Proj, m_View;
    };
}