#include "texture.h"
#define STB_IMAGE_IMPLEMENTATION 1
#include "stb_image.h"
#include <iostream>

Texture::Texture(/* args */)
    : m_GLTexture(0)
{
}

Texture::~Texture()
{
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
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glBindTexture(GL_TEXTURE_2D, 0);        
    }
    else
    {
        std::cout << "Load " << file << "failed" << std::endl;
        return;
    }
    
    stbi_image_free(data);
}