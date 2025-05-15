#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

// texture samplers
uniform sampler2D plano0;

void main()
{
	FragColor = texture(plano0, TexCoord);
}

