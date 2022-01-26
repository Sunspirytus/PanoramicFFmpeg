#version 410 core

#define PI 3.1415926

uniform sampler2D inTextureY;
uniform sampler2D inTextureU;
uniform sampler2D inTextureV;

layout (location = 0) in vec2 varyingUV;
layout (location = 1) in vec3 varyingParaSampleDirection;
out vec4 FragColor;

void main()
{
    vec3 SampleDir = normalize(varyingParaSampleDirection);

    float V = acos(SampleDir.y) / PI / 2;
    float U = atan(SampleDir.z, SampleDir.x) / 2.0 / PI;
    //U = fract(U);

    float FrameY = texture(inTextureY, vec2(U, V)).r;
    float FrameU = texture(inTextureU, vec2(U, V)).r;
    float FrameV = texture(inTextureV, vec2(U, V)).r;

    float R = FrameY + 1.4075 * (FrameV - 0.5);
    float G = FrameY - 0.3455 * (FrameU - 0.5) - 0.7169 * (FrameV - 0.5);
    float B = FrameY + 1.779 * (FrameU - 0.5);

    FragColor = vec4(R, G, B, 1.0);
}