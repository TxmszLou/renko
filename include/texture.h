#pragma once

#include "renderer.h"

class Texture
{
private:
    uint32_t m_RendererID;
    std::string m_FilePath;
    unsigned char* m_LocalBuffer; // store the texture
    int m_Width, m_Height, m_BPP; // bits per pixel

public:
    Texture(const std::string& path);
    ~Texture();

    void Bind(uint32_t slot = 0) const;
    void Unbind() const;

    inline int GetWidth() const { return m_Width; }
    inline int GetHeight() const { return m_Height; }
};