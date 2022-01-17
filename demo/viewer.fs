#version 410 core
uniform sampler2D inTexture;

layout (location = 0) in vec2 varyingUV;
out vec4 FragColor;

void main()
{
    FragColor = texture(inTexture, varyingUV);
    //FragColor = vec4(varyingUV, 0.0, 1.0);
}