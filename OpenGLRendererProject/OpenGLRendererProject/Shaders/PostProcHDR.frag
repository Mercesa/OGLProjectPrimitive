#version 430 core

out vec4 color;
in vec2 outTexC;

uniform sampler2D hdrBuffer;
uniform sampler2D brightnessBuffer;

// Define gamma so it compiles into a literal (instead of redoing the calculation over and over again)
#define GAMMA 1.0/1.01


void main()
{	
	vec3 hdrColor = texture(hdrBuffer, outTexC).rgb;
	vec3 newCol = texture(brightnessBuffer, outTexC).rgb + hdrColor;
	vec3 toneMapped = newCol / (newCol + vec3(1.0));
	newCol = pow(newCol.rgb, vec3(1.0/GAMMA));
	color = vec4(newCol, 1.0);
}	