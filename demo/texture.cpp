#include "texture.h"
#define STB_IMAGE_IMPLEMENTATION 1
#include "stb_image.h"
#include <iostream>

Texture::Texture(/* args */)
    : m_GLTexture(0)
    , m_Width(0)
    , m_Height(0)
    , m_Format(TextureFormat::None)
{
}

Texture::~Texture()
{
    glDeleteTextures(1, &m_GLTexture);
}

void Texture::LoadTexture(const char* file)
{
    int32_t width;
    int32_t height;
    int32_t channels;
    uint8_t* data = stbi_load(file, &width, &height, &channels, 4);
    if (data)
    {
        glGenTextures(1, &m_GLTexture);
        glBindTexture(GL_TEXTURE_2D, m_GLTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glBindTexture(GL_TEXTURE_2D, 0);

        m_Width = width;
        m_Height = height;
        m_Format = TextureFormat::RGBA8;
    }
    else
    {
        std::cout << "Load " << file << "failed" << std::endl;
        return;
    }
    
    stbi_image_free(data);
}

void Texture::Create(uint32_t width, uint32_t height, TextureFormat format, void* data)
{
    m_Width = width;
    m_Height = height;
    m_Format = format;

    glGenTextures(1, &m_GLTexture);
    glBindTexture(GL_TEXTURE_2D, m_GLTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GetGLFormat(), width, height, 0, GetGLFormat(), GetGlElement(), data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::UpdateTextureData(void* data)
{
    glBindTexture(GL_TEXTURE_2D, m_GLTexture);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_Width, m_Height, GetGLFormat(), GetGlElement(), data);
    glBindTexture(GL_TEXTURE_2D, 0);
}

GLsizei Texture::GetGLFormat()
{
    GLsizei GLFormat;
    switch (m_Format) 
    {
    case TextureFormat::R8:
        GLFormat = GL_RED;
        break;
    case TextureFormat::RGBA8:
        GLFormat = GL_RGBA;
        break;
    default:
        break;
    }
    return GLFormat;
}

GLenum Texture::GetGlElement()
{
    GLenum GLDataSize;
    switch (m_Format) 
    {
    case TextureFormat::R8:
        GLDataSize = GL_UNSIGNED_BYTE;
        break;
    case TextureFormat::RGBA8:
        GLDataSize = GL_UNSIGNED_BYTE;
        break;
    default:
        break;
    }
    return GLDataSize;
}