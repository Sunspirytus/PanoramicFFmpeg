#pragma once
#include "glad/glad.h"

class Texture
{
public:
    Texture(/* args */);
    ~Texture();

    void LoadTexture(const char* file);
    GLuint GetGlTexture() { return m_GLTexture; }

private:
    GLuint m_GLTexture;
};

