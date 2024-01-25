#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "testBatchRenderer.h"
#include "batchRenderer.h"

#include <iostream>

namespace test
{
    TestBatchRenderer::TestBatchRenderer()
        : m_Proj(glm::ortho(0.f, 540.f, 0.f, 960.f, -1.f, 1.f)),
          m_View(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0))),
          m_Translation(200, 200, 0)
    {
        // enable alpha
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
        GLCall(glEnable(GL_BLEND));

        BatchRenderer::Init();

        m_Textures.push_back(std::make_unique<Texture>("../res/textures/vsc-logo.png"));
        m_Textures.push_back(std::make_unique<Texture>("../res/textures/google-logo.png"));

        m_Shader = std::make_unique<Shader>("../res/shaders/TestBatchRenderer.shader");
        m_Shader->Bind();

        int samplers[32];
        for (int i = 0; i < 32; i++)
            samplers[i] = i;
        
        m_Shader->SetUniform1iv("u_Textures", 32, samplers);
    }

    TestBatchRenderer::~TestBatchRenderer()
    {
        BatchRenderer::Shutdown();
    }

    void TestBatchRenderer::OnUpdate(float deltaTime)
    {

        GLCall(glClearColor(0.f, 0.f, 0.f, 1.f));
        GLCall(glClear(GL_COLOR_BUFFER_BIT));

        m_Shader->Bind();

        BatchRenderer::BeginBatch();

        if (ImGui::Button("New Quad"))
            m_NumOfQuads++;

        float size = 30.f;
        for (int i = 0; i < m_NumOfQuads / m_QuadsPerRow; i++)
            for (int j = 0; j <= m_QuadsPerRow; j++)
            {
                int texSlot = (i + j) % 2;
                BatchRenderer::MakeQuad(
                     { j * size, i * size },
                     { size, size },
                      m_Textures[texSlot]->GetRendererID());
            }
        for (int j = 0; j < m_NumOfQuads % m_QuadsPerRow; j++)
        {
            int texSlot = j % 2;
            BatchRenderer::MakeQuad(
                { j * size, (m_NumOfQuads / m_QuadsPerRow) * size },
                { size, size },
                m_Textures[texSlot]->GetRendererID());
        }


        glm::mat4 model = glm::translate(glm::mat4(1.0f), m_Translation);
        glm::mat4 mvp = m_Proj * m_View * model;

        // flush data and draw
        m_Shader->SetUniformMat4f("u_MVP", mvp);
        BatchRenderer::EndBatch();
        BatchRenderer::Flush();
    }
    
    void TestBatchRenderer::OnRender()
    {
    }

    void TestBatchRenderer::OnImGuiRender()
    {
        // change the position of quad by dynamically creating vertex array
        // ImGui::DragFloat2("Quad Position", m_QuadPosition, 5.f);
        // // change the position of quad by changing the model matrix
        ImGui::SliderFloat3("Translation", &m_Translation.x, 0.0f, 540.0f);
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    }
}