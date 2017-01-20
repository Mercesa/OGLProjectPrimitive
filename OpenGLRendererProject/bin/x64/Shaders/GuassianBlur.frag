#version 450 core

out vec4 color;
in vec2 outTexC;

uniform sampler2D image;

uniform bool horizontal;

uniform float weight[5] = float[] (0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216);
void main()
{	
	vec2 tex_offset = 1.0 / textureSize(image, 0);
	vec3 result = texture(image, outTexC);
	if(horizontal)
	{
		for(int i = 0; i < 5; i++)
		{
			result += texture(image, vec2(outTexC.x + tex_offset.x * i, 0.0f).rgb * weight[i];
			result += texture(image, vec2(outTexC.x - tex_offset.x * i, 0.0f).rgb * weight[i];

		}
	}
	else
	{
		for(int i = 0; i < 5; i++)
		{
			result += texture(image, vec2(0.0f, outTexC.y + tex_offset.y * i).rgb * weight[i];
			result += texture(image, vec2(0.0f, outTexC.y - tex_offset.y * i).rgb * weight[i];

		}
	}

	color = vec4(result, 1.0f);
}	