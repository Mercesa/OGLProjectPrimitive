#version 450 core


struct light
{
	vec3 position;
	vec3 DiffuseColor;
	vec3 SpecularColor;
};

in vec2 outTexC;
in vec3 outFragPosition;
in vec3 outNormal;

out vec4 color;

vec3 lightPosition = vec3(0.0f, 5.0f, 0.0f);

uniform sampler2D tex;

// have frag position
// vector from frag to light position
// normalize that vec
// dot product normal and vec from frag to light pos
// end
uniform light Lights[1];

void main()
{

	for(int i = 0; i < 1; i++)
	{
		// vec from lightpos to frag pos
		vec3 L = Lights[i].position - outFragPosition;
		L = normalize(L);
		float dotProdLightNormal = dot(L, outNormal);

		color = texture(tex, outTexC) * dotProdLightNormal;
	}	

}	