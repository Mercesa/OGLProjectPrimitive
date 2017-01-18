#version 450 core

out vec4 color;
in vec2 outTexC;

uniform sampler2D hdrBuffer;

// Define gamma so it compiles into a literal (instead of redoing the calculation over and over again)
#define GAMMA 1.0/1.01


void main()
{	
	vec3 hdrColor = texture(hdrBuffer, outTexC).rgb;
	
	vec3 toneMapped = hdrColor / (hdrColor + vec3(1.0));
	toneMapped = pow(toneMapped.rgb, vec3(1.0/GAMMA));
	color = vec4(toneMapped, 1.0);
	
}	