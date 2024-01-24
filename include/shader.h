#pragma once

#include <string>
#include <unordered_map>
#include <glm/glm.hpp>

struct ShaderProgramSource
{
    std::string VertexSource;
    std::string FragmentSource;
};

class Shader
{
public:
    Shader(const std::string& filepath);
    ~Shader();

    void Bind() const;
    void Unbind() const;

    // Set Uniforms
    void SetUniform1i(const std::string& name, int value);
    void SetUniform1iv(const std::string& name, int count, int* values);
    void SetUniform1f(const std::string& name, float value);
    void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
    void SetUniformMat4f(const std::string& name, const glm::mat4& matrix);

private:
    std::unordered_map<std::string, int> m_UniformLocationCache;
    uint32_t m_RendererID;
    std::string m_FilePath;
    // caching uniform locations
    uint32_t GetUniformLocation(const std::string& name);
    uint32_t CompileShader(uint32_t type, const std::string& source);
    uint32_t CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
    ShaderProgramSource ParseShader(const std::string& filepath);
};