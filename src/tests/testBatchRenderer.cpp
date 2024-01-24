#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "testBatchRenderer.h"
#include "batchRenderer.h"

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

        m_Shader = std::make_unique<Shader>("../res/shaders/TestBatchRenderer.shader");
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

        BatchRenderer::MakeQuad(0.f, 0.f, 100.f, 100.f, glm::vec4(1.0,1.0,1.0,1.0));
        // if (ImGui::Button("New Quad")) {
        //     BatchRenderer::MakeQuad(m_NextQuadPX, m_NextQuadPY, 10.f, 10.f, glm::vec4(1.0,1.0,1.0,1.0));
        //     if (m_NextQuadPX > m_QuadsPerRow * 10.f)
        //     {
        //         m_NextQuadPX = 0.f;
        //         m_NextQuadPY += 10.f;
        //     } else
        //         m_NextQuadPX += 10.f;
        // }
        BatchRenderer::EndBatch();

        glm::mat4 model = glm::translate(glm::mat4(1.0f), m_Translation);
        glm::mat4 mvp = m_Proj * m_View * model;

        // flush data and draw
        m_Shader->SetUniformMat4f("u_MVP", mvp);
        m_Shader->SetUniform4f("u_Color", 0.2f, 0.4f, 0.6f, 1.f);
        BatchRenderer::Flush();
    }
    
    void TestBatchRenderer::OnRender()
    {
    }

    void TestBatchRenderer::OnImGuiRender()
    {
        ImGui::Begin("Controls");
        // change the position of quad by dynamically creating vertex array
        // ImGui::DragFloat2("Quad Position", m_QuadPosition, 5.f);
        // // change the position of quad by changing the model matrix
        ImGui::SliderFloat3("Translation", &m_Translation.x, 0.0f, 540.0f);
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::End();
    }
}