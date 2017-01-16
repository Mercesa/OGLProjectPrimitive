#version 450 core


uniform vec3 lightColor;
out vec4 color;

void main()
{
	color = vec4(lightColor.rgb, 1.0f);		
}	