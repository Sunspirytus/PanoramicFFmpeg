#pragma once
#include "glad/glad.h"

enum class TextureFormat
{
    None,
    R8,
    RGBA8,
};

class Texture
{
public:
    Texture(/* args */);
    ~Texture();

    void LoadTexture(const char* file);
    GLuint GetGlTexture() { return m_GLTexture; }

    void Create(uint32_t width, uint32_t height, TextureFormat format, void* data);
    void UpdateTextureData(void* data);

private:
    GLsizei GetGLFormat();
    GLenum GetGlElement();

private:
    GLuint m_GLTexture;
    uint32_t m_Width;
    uint32_t m_Height;
    TextureFormat m_Format;
};

