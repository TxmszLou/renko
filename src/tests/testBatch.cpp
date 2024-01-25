#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "tests/testBatch.h"


namespace test
{
    struct Vertex
    {
        glm::vec2 position;
        glm::vec2 texCoord;
    };

    // lower left corner at (px, py), width = size
    static std::array<Vertex, 4> MakeQuad(float px, float py) {
        float size = 100.f;
        Vertex v0;
        v0.position = glm::vec2(px, py);
        v0.texCoord = glm::vec2(0.f, 0.f);

        Vertex v1;
        v1.position = glm::vec2(px + size, py);
        v1.texCoord = glm::vec2(1.f, 0.f);

        Vertex v2;
        v2.position = glm::vec2(px + size, py + size);
        v2.texCoord = glm::vec2(1.f, 1.f);

        Vertex v3;
        v3.position = glm::vec2(px, py + size);
        v3.texCoord = glm::vec2(0.f, 1.f);

        return { v0, v1, v2, v3 };
    }

    TestBatch::TestBatch()
        : m_Proj(glm::ortho(0.f, 540.f, 0.f, 960.f, -1.f, 1.f)),
          m_View(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0))),
          m_Translation(200, 200, 0)
    {
        // enable alpha
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
        GLCall(glEnable(GL_BLEND));

        // make a quad
        auto q0 = MakeQuad(0, 0);
        Vertex vertices[4];
        memcpy(vertices, q0.data(), q0.size() * sizeof(Vertex));

        uint32_t indices[] = {
            0, 1, 2, // first triangle
            2, 3, 0,  // second one
            4, 5, 6,
            6, 7, 4
        };

        m_VAO = std::make_unique<VertexArray>();
        m_VAO->Init();
        m_IBO = std::make_unique<IndexBuffer>();
        m_IBO->Init(indices, 12);
        m_Shader = std::make_unique<Shader>("../res/shaders/TestBatch.shader");

        m_Textures.push_back(std::make_unique<Texture>("../res/textures/vsc-logo.png"));

        // make a dynamic vertex buffer by not passing the data
        m_VB  = std::make_unique<VertexBuffer>();
        m_VB->Init(sizeof(Vertex) * 1000);

        VertexBufferLayout layout;
        layout.Push<glm::vec2>(1); // vertex coord
        layout.Push<glm::vec2>(1); // tex coord
        m_VAO->AddBuffer(*m_VB, layout);

        m_Textures[0]->Bind(0);


        m_VAO->Unbind();
        m_Shader->Unbind();
        m_VB->Unbind();
        m_IBO->Unbind();
    }

    TestBatch::~TestBatch()
    {
    }

    void TestBatch::OnUpdate(float deltaTime)
    {
        // enable alpha
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
        GLCall(glEnable(GL_BLEND));

        // make a quad
        auto q0 = MakeQuad( m_QuadPosition[0], m_QuadPosition[1] );
        auto q1 = MakeQuad(100, 0);
        Vertex vertices[8];
        memcpy(vertices, q0.data(), q0.size() * sizeof(Vertex));
        memcpy(vertices + q0.size(), q1.data(), q1.size() * sizeof(Vertex));

        VertexBufferLayout layout;
        layout.Push<float>(2); // vertex coord
        layout.Push<float>(2); // tex coord
        m_VAO->AddBuffer(*m_VB, layout);

        // pushdata
        GLCall(glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices));

        m_Textures[0]->Bind(0);


        m_VAO->Unbind();
        m_Shader->Unbind();
        m_VB->Unbind();
        m_IBO->Unbind();
    }

    void TestBatch::OnRender()
    {
        GLCall(glClearColor(0.f, 0.f, 0.f, 1.f));
        GLCall(glClear(GL_COLOR_BUFFER_BIT));

        int py = 0;
        Renderer renderer;
        {
            // draw the first square
            glm::mat4 model = glm::translate(glm::mat4(1.0f), m_Translation);
            glm::mat4 mvp = m_Proj * m_View * model;

            m_Shader->Bind();
            // m_Shader->SetUniform4f("u_Color", 0.2f, 0.4f, 0.6f, 1.f);
            m_Shader->SetUniformMat4f("u_MVP", mvp);
            renderer.Draw(*m_VAO, *m_IBO, *m_Shader);
        }

    }

    void TestBatch::OnImGuiRender()
    {
        // change the position of quad by dynamically creating vertex array
        ImGui::DragFloat2("Quad Position", m_QuadPosition, 5.f);
        // change the position of quad by changing the model matrix
        ImGui::SliderFloat3("Translation", &m_Translation.x, 0.0f, 540.0f);
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    }
}
