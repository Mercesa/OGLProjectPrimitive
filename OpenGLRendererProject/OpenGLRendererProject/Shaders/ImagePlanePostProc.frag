#version 430 core

out vec4 color;
in vec2 outTexC;

uniform sampler2D aTexture;

void main()
{	
	color = texture(aTexture, outTexC);
}	