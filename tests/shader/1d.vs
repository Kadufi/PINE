#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in float aTex; // textura 1D

out float texCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    texCoord = aTex;
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}
