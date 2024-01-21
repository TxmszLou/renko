#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <linmath.h>

#include <iostream>
#include <fstream>
#include <sstream>

#include <renko/core.h>
#include <renko/particle.h>

#include <renderer.h>
#include "vertexBuffer.h"
#include "indexBuffer.h"

using namespace renko;

struct ShaderProgramSource
{
    std::string VertexSource;
    std::string FragmentSource;
};

static ShaderProgramSource ParseShader(const std::string& filepath)
{
    std::ifstream stream(filepath);

    enum class ShaderType
    {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };

    std::string line;
    // one for vertex shader, one for fragment shader
    std::stringstream ss[2];
    ShaderType type = ShaderType::NONE;

    while (getline(stream, line))
    {
        if (line.find("#shader") != std::string::npos)
        {
            if (line.find("vertex") != std::string::npos)
                type = ShaderType::VERTEX;
            else if (line.find("fragment") != std::string::npos)
                type = ShaderType::FRAGMENT;
        }
        else
        {
            ss[(int)type] << line << '\n';
        }
    }

    return { ss[0].str(), ss[1].str() };
}

static unsigned int CompileShader(unsigned int type, const std::string& source)
{
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str(); // didn't copy the string, make sure source still exists
    GLCall(glShaderSource(id, 1, &src, nullptr));
    GLCall(glCompileShader(id));

    // Error handling
    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE)
    {
        // get error message
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*) alloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "Fail to compile " <<
         (type == GL_VERTEX_SHADER ? "vertex shader" : "fragment shader") << std::endl;
        std::cout << message << std::endl;
        glDeleteShader(id);
        return 0;
    }

    return id;
}

static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    // attach shaders
    GLCall(glAttachShader(program, vs));
    GLCall(glAttachShader(program, fs));
    GLCall(glLinkProgram(program));
    GLCall(glValidateProgram(program));

    // delete intermediate complie files
    GLCall(glDeleteShader(vs));
    GLCall(glDeleteShader(fs));

    return program;
}

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
    unsigned int vertex_buffer, vertex_array_object, index_buffer_object, vpos_location, vcol_location, fcol_location;

    glfwSetErrorCallback(error_callback);

    /* Initialize the library */
    if (!glfwInit())
        exit(EXIT_FAILURE);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
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
        // float r, g, b;
    } vertices[4] =
    {
        { -0.5f, -0.5f}, // , 1.f, 0.f, 0.f}, // 0
        {  0.5f, -0.5f}, //, 0.f, 1.f, 0.f}, // 1
        {  0.5f, 0.5f}, //, 0.f, 0.f, 1.f},  // 2
        { -0.5f, 0.5f} //, 1.f, 1.f, 1.f}   // 3
    };

    unsigned int indices[] = {
        0, 1, 2, // first triangle
        2, 3, 0  // second one
    };

    GLCall(glGenVertexArrays(1, &vertex_array_object));
    GLCall(glBindVertexArray(vertex_array_object));

    VertexBuffer vb(vertices, sizeof(vertices));
    // GLCall(glGenBuffers(1, &vertex_buffer));
    // GLCall(glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer)); // select the buffer, tells the GPU buffer is just an array
    // GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW)); // copy data to vertex buffer

    IndexBuffer ib(indices, 6);
    // GLCall(glGenBuffers(1, &index_buffer_object));
    // GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer_object)); // select the buffer, tells the GPU buffer is just an array
    // GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW)); // copy data to vertex buffer

    // read the shader!
    ShaderProgramSource source = ParseShader("../res/shaders/Basic.shader");

    unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource);

    // GLCall(mvp_location = glGetUniformLocation(shader, "MVP"));
    GLCall(vpos_location = glGetAttribLocation(shader, "vPos"));
    // GLCall(vcol_location = glGetAttribLocation(shader, "vCol"));
    GLCall(fcol_location = glGetUniformLocation(shader, "u_Color"));

    // enable vertex attribute
    GLCall(glEnableVertexAttribArray(vpos_location));
    // set vertex attribute
    // this will bind the vertex array buffer to the currently bound vertex array object
    GLCall(glVertexAttribPointer(vpos_location, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (void*) 0));

    // GLCall(glEnableVertexAttribArray(vcol_location));
    // GLCall(glVertexAttribPointer(vcol_location, 3, GL_FLOAT, GL_FALSE, sizeof(vertices[0]), (void*) (sizeof(float) * 2)));


    // r value to change over time
    float r = 0.0f;
    float increment = 0.05f;
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        // float ratio;
        // int width, height;
        // mat4x4 m;

        // glfwGetFramebufferSize(window, &width, &height);
        // ratio = width / (float) height;

        /* Render here */
        // glViewport(0, 0, width, height); 
        glClear(GL_COLOR_BUFFER_BIT);
        GLClearError();

        GLCall(glUseProgram(shader));

        // mat4x4_identity(m);
        // mat4x4_rotate_Z(m, m, (float) 0);
        // mat4x4_mul(mvp, p, m);

        // GLCall(glUniformMatrix4fv(mvp_location, 1, GL_FALSE, (const GLfloat*) m));
        GLCall(glUniform4f(fcol_location, r, 0.3f, 0.8f, 1.0f));

        GLCall(glBindVertexArray(vertex_array_object));
        ib.Bind();

        GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));
        // glDrawArrays(GL_TRIANGLES, 0, 3);

        if (r > 1.0f)
            increment = -0.05f;
        else if (r < 0.0f)
            increment = 0.05f;

        r += increment;


        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glDeleteProgram(shader);
    glfwTerminate();
    return 0;
}