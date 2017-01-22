#version 450 core

layout(location = 0) out vec3 color;
layout(location = 1) out vec3 color2;

uniform vec3 lightColor;

void main()
{
	color = vec3(lightColor.rgb);		
	color2 = vec3(lightColor.rgb);		

}	