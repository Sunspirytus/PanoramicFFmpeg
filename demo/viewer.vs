#version 410 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aUV;

uniform vec3 CornerDirection0;
uniform vec3 CornerDirection1;
uniform vec3 CornerDirection2;
uniform vec3 CornerDirection3;

// uniform mat4 model;
// uniform mat4 view;
// uniform mat4 projection;

layout (location = 0) out vec2 varyingUV;
layout (location = 1) out vec3 varyingParaSampleDirection;

void main()
{
	//gl_Position = projection * view * model * vec4(aPos, 1.0);
    varyingUV = aUV;

    vec3 DirU0 = mix(CornerDirection0, CornerDirection1, aUV.x);
    vec3 DirU1 = mix(CornerDirection2, CornerDirection3, aUV.x);
    varyingParaSampleDirection = mix(DirU0, DirU1, aUV.y);

    gl_Position = vec4(aPos, 1.0);
}