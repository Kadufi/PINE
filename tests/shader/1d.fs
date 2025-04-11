#version 330 core
out vec4 FragColor;

in float texCoord;

uniform sampler1D texture1D;

void main()
{
    FragColor = texture(texture1D, texCoord);
}
