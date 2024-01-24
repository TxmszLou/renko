#pragma once

#include "tests/test.h"

#include "vertexArray.h"
#include "vertexBuffer.h"
#include "vertexBufferLayout.h"
#include "texture.h"

#include <memory>

namespace test {
    // making a quad with solid color
    class TestBatchRenderer : public Test {
    public:
        TestBatchRenderer();
        ~TestBatchRenderer();

        void OnUpdate(float deltaTime) override;
        void OnRender() override;
        void OnImGuiRender() override;

    private:
        // translation matrix
        glm::vec3 m_Translation;
        glm::mat4 m_Proj, m_View;
        std::unique_ptr<Shader> m_Shader;

        int m_QuadsPerRow = 10;
        float m_NextQuadPX = 0.0;
        float m_NextQuadPY = 0.0;
        int m_NumOfQuads = 0;
    };
}