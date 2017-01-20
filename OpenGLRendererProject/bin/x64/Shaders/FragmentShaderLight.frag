#version 450 core

layout(location = 0) out vec3 color;

uniform vec3 lightColor;

void main()
{
	color = vec3(lightColor.rgb);		
}	