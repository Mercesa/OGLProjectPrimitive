#version 450 core

out vec4 color;
in vec2 outTexC;

uniform sampler2D hdrBuffer;

void main()
{	
	vec3 hdrColor = texture(hdrBuffer, outTexC).rgb;
	color = vec4(hdrColor, 1.0);
	
	//color = vec4(1.0f, 0.0f, 0.0f, 1.0f);
}	