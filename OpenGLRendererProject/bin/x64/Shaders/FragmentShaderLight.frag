#version 450 core

layout(location = 0) out vec4 color;

uniform vec3 lightColor;

void main()
{
	color = vec4(lightColor.rgb, 1.0f);		
}	