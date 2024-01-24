
#include <iostream>
#include <fstream>
#include <sstream>

#include <renko/core.h>
#include <renko/particle.h>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "renderer.h"
#include "vertexBuffer.h"
#include "vertexBufferLayout.h"
#include "indexBuffer.h"
#include "vertexArray.h"
#include "shader.h"
#include "texture.h"

#include "tests/testClearColor.h"
#include "tests/testQuadSolid.h"
#include "tests/testTexQuad.h"
#include "tests/testBatch.h"
#include "tests/testBatchRenderer.h"

using namespace renko;


static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}
 
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

int main (int argc, char **argv) {
    GLFWwindow* window;

    glfwSetErrorCallback(error_callback);

    /* Initialize the library */
    if (!glfwInit())
        exit(EXIT_FAILURE);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(540, 960, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwSetKeyCallback(window, key_callback);

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    gladLoadGL();
    glfwSwapInterval(1);

    std::cout << glGetString(GL_VERSION) << std::endl;

    // enable alpha
    GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
    GLCall(glEnable(GL_BLEND));

    // Init ImGUI
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init();

    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    ImGui::StyleColorsDark();

    {
        // make a scope for the tests
        // the tests should get deleted out of this scope by the destructors
        test::Test* currentTest = nullptr;
        test::TestMenu* testMenu = new test::TestMenu(currentTest);
        currentTest = testMenu;

        testMenu->RegisterTest<test::TestClearColor>("Clear Color");
        testMenu->RegisterTest<test::TestQuadSolid>("Solid Quad");
        testMenu->RegisterTest<test::TestTexQuad>("Texture Quad");
        testMenu->RegisterTest<test::TestBatch>("Batch Quad");
        testMenu->RegisterTest<test::TestBatchRenderer>("Batch Renderer Quad");

        while (!glfwWindowShouldClose(window))
        {
            // clear the background to black
            GLCall(glClearColor(0.0, 0.0, 0.0, 1.0));
            GLCall(glClear(GL_COLOR_BUFFER_BIT));

            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            if (currentTest)
            {
                currentTest->OnUpdate(0.0f);
                currentTest->OnRender();
                ImGui::Begin("Test");

                if (currentTest != testMenu && ImGui::Button("<-"))
                {
                    // free the current test when "back"
                    delete currentTest;
                    currentTest = testMenu;
                }

                currentTest->OnImGuiRender();

                ImGui::End();
            }

            // ImGUI render
            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();
        }
        delete currentTest;
        if (currentTest != testMenu)
            delete testMenu;
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}