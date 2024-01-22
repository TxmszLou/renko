
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
    /* Vector3 myVector(1,1,1); */

    /* std::cout << myVector << std::endl; */
    /* std::cout << "magnitute: " << myVector.magnitute() << std::endl; */
    /* std::cout << "1/magnitute: " << (real)1/myVector.magnitute() << std::endl; */

    /* Vector3 myVectorNormalized = myVector / myVector.magnitute(); */

    /* myVector.normalize(); */
    /* std::cout << myVector << std::endl; */
    /* std::cout << myVectorNormalized << std::endl; */
    /* std::cout << "magnitute: " << myVectorNormalized.magnitute() << std::endl; */

    /* Vector3 i(1,0,0); */
    /* Vector3 j(0,1,0); */
    /* Vector3 k = i.crossProduct(j); */
    /* std::cout << k << std::endl; */
    /* std::cout << i << " x " << j << " = " << i.crossProduct(j) << std::endl; */

    // Particle p {};
    // p.totalForce = Vector3(0,-10,0);
    // p.setMass(10.0);
    // p.damping = 1;
    // std::cout << p << std::endl;

    // for (int t = 0; t < 50; t++) {
    //     std::cout << p << std::endl;
    //     std::cout << p.totalForce << std::endl;
    //     p.integrate((real)1);
    // }

    GLFWwindow* window;
    unsigned int vertex_buffer, vertex_array_object, index_buffer_object;

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

    // three vertices of the triangle
    static const struct {
        float x, y;
        float texCoord_x, texCoord_y;
        // float r, g, b;
    } vertices[4] =
    {
        { -50.f,-50.f, 0.f, 0.f}, // , 1.f, 0.f, 0.f}, // 0
        { 50.f, -50.f, 1.f, 0.f}, //, 0.f, 1.f, 0.f}, // 1
        { 50.f, 50.f,  1.f, 1.f}, //, 0.f, 0.f, 1.f},  // 2
        { -50.f,  50.f,  0.f, 1.f} //, 1.f, 1.f, 1.f}   // 3
    };

    unsigned int indices[] = {
        0, 1, 2, // first triangle
        2, 3, 0  // second one
    };


    // enable alpha
    GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
    GLCall(glEnable(GL_BLEND));

    // Want to also abstract vertex array into something like
    // to track the layout for buffer
    VertexArray va;
    VertexBuffer vb(vertices, sizeof(vertices));
    VertexBufferLayout layout;
    layout.Push<float>(2); // vertex coord
    layout.Push<float>(2); // tex coord
    va.AddBuffer(vb, layout);

    IndexBuffer ib(indices, 6);

    // make projection matrix
    // left, right, bottom, top, near, far
    glm::mat4 proj = glm::ortho(0.f, 540.f, 0.f, 960.f, -1.f, 1.f);
    glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));

    // read the shader!
    Shader shader("../res/shaders/Basic.shader");
    shader.Bind();

    Texture texture("../res/textures/Logo.png");
    texture.Bind();
    shader.SetUniform1i("u_Texture", 0); // slot 0

    va.Unbind();
    shader.Unbind();
    vb.Unbind();
    ib.Unbind();

    Renderer renderer;

    // Init ImGUI
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init();

    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    ImGui::StyleColorsDark();

    bool show_demo_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    // translation matrix
    glm::vec3 translationA(200, 200, 0);
    glm::vec3 translationB(400, 200, 0);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        // float ratio;
        // int width, height;
        // mat4x4 m;

        // glfwGetFramebufferSize(window, &width, &height);
        // ratio = width / (float) height;

        /* Render here */
        renderer.Clear();

        // Start the ImGUI frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        {
            // draw the first square
            glm::mat4 model = glm::translate(glm::mat4(1.0f), translationA);
            glm::mat4 mvp = proj * view * model;

            shader.Bind();
            shader.SetUniformMat4f("u_MVP", mvp);
            renderer.Draw(va, ib, shader);
        }

        {
            // draw the second square
            glm::mat4 model = glm::translate(glm::mat4(1.0f), translationB);
            glm::mat4 mvp = proj * view * model;

            shader.Bind();
            shader.SetUniformMat4f("u_MVP", mvp);
            renderer.Draw(va, ib, shader);
        }

        // ImGui window
        {
            ImGui::Begin("Hello, world!");
            ImGui::SliderFloat3("Translation A", &translationA.x, 0.0f, 540.0f);
            ImGui::SliderFloat3("Translation B", &translationB.x, 0.0f, 540.0f);
            // ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

            // if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
            //     counter++;
            // ImGui::SameLine();
            // ImGui::Text("counter = %d", counter);

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
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

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}