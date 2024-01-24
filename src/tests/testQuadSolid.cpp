#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "tests/testQuadSolid.h"


namespace test
{

    TestQuadSolid::TestQuadSolid()
        : m_Proj(glm::ortho(0.f, 540.f, 0.f, 960.f, -1.f, 1.f)),
          m_View(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0))),
          m_TranslationA(200, 200, 0), m_TranslationB(400, 200, 0)
    {
        // three vertices of the triangle
        const struct
        {
            float x, y;
            float texCoord_x, texCoord_y;
            // float r, g, b;
        } vertices[4] =
            {
                {-50.f, -50.f, 0.f, 0.f}, // , 1.f, 0.f, 0.f}, // 0
                {50.f, -50.f, 1.f, 0.f},  //, 0.f, 1.f, 0.f}, // 1
                {50.f, 50.f, 1.f, 1.f},   //, 0.f, 0.f, 1.f},  // 2
                {-50.f, 50.f, 0.f, 1.f}   //, 1.f, 1.f, 1.f}   // 3
            };

        uint32_t indices[] = {
            0, 1, 2, // first triangle
            2, 3, 0  // second one
        };

        // enable alpha
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
        GLCall(glEnable(GL_BLEND));

        m_VAO = std::make_unique<VertexArray>();
        m_VAO->Init();
        m_IBO = std::make_unique<IndexBuffer>();
        m_IBO->Init(indices, 6);
        m_VB  = std::make_unique<VertexBuffer>();
        m_VB->Init(vertices, sizeof(vertices));
        m_Shader = std::make_unique<Shader>("../res/shaders/TestQuadSolid.shader");

        VertexBufferLayout layout;
        layout.Push<float>(2); // vertex coord
        layout.Push<float>(2); // tex coord
        m_VAO->AddBuffer(*m_VB, layout);
        m_Shader->Bind();

        m_VAO->Unbind();
        m_Shader->Unbind();
        m_VB->Unbind();
        m_IBO->Unbind();
    }

    TestQuadSolid::~TestQuadSolid()
    {
    }

    void TestQuadSolid::OnUpdate(float deltaTime)
    {
    }

    void TestQuadSolid::OnRender()
    {
        GLCall(glClearColor(0.f, 0.f, 0.f, 1.f));
        GLCall(glClear(GL_COLOR_BUFFER_BIT));

        Renderer renderer;
        {
            // draw the first square
            glm::mat4 model = glm::translate(glm::mat4(1.0f), m_TranslationA);
            glm::mat4 mvp = m_Proj * m_View * model;

            m_Shader->Bind();
            m_Shader->SetUniform4f("u_Color", 0.2f, 0.4f, 0.6f, 1.f);
            m_Shader->SetUniformMat4f("u_MVP", mvp);
            renderer.Draw(*m_VAO, *m_IBO, *m_Shader);
        }


        {
            // draw the second square
            glm::mat4 model = glm::translate(glm::mat4(1.0f), m_TranslationB);
            glm::mat4 mvp = m_Proj * m_View * model;

            m_Shader->Bind();
            m_Shader->SetUniform4f("u_Color", 0.2f, 0.4f, 0.f, 1.f);
            m_Shader->SetUniformMat4f("u_MVP", mvp);
            renderer.Draw(*m_VAO, *m_IBO, *m_Shader);
        }

    }

    void TestQuadSolid::OnImGuiRender()
    {
        ImGui::SliderFloat3("Translation A", &m_TranslationA.x, 0.0f, 540.0f);
        ImGui::SliderFloat3("Translation B", &m_TranslationB.x, 0.0f, 540.0f);
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    }
}
