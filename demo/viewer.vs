#version 410 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aUV;

// uniform mat4 model;
// uniform mat4 view;
// uniform mat4 projection;

layout (location = 0) out vec2 varyingUV;

void main()
{
	//gl_Position = projection * view * model * vec4(aPos, 1.0);
    varyingUV = aUV;

    gl_Position = vec4(aPos, 1.0);
}