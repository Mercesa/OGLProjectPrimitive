#version 450 core

// Write to RGBA buffer
layout(location = 0) out vec3 color;

struct light
{
	vec3 position;
	vec3 DiffuseColor;
	vec3 SpecularColor;
};

in vec2 outTexC;
in vec3 outFragPosition;
in vec3 outNormal;


uniform sampler2D diffuseTexture;
uniform vec3 cameraPosition;

uniform int usePhong;
uniform int amountOfLights;

// The literal should be replaced with either a define or constant
uniform light Lights[256];

void main()
{	
	vec4 ambient = vec4(0.0f) * texture(diffuseTexture, outTexC);
	
	// Frag to eye vector
	vec3 FE = normalize(cameraPosition - outFragPosition);

	for(int i = 0; i < amountOfLights; i++)
	{	
		// Vector from frag position to light position
		vec3 L = Lights[i].position - outFragPosition;
		float distance = length(L);
		float attenuation = 1.0f / (1.0 + 0.7 * distance + 1.8 * (distance * distance));   
		L = normalize(L);
		float dotProdLightNormal = max(dot(L, outNormal), 0.0f);


		float specDiff = 0.0f;

		// blinn lighting

		if(usePhong == 0)
		{
			vec3 halfWayDir = normalize(FE+L);
			specDiff = pow(max(dot(outNormal, halfWayDir), 0.0), 128);
		}
		
		if(usePhong == 1)
		{
			vec3 invL = -L;
			vec3 R = reflect(invL, outNormal);
			vec3 FE = normalize(cameraPosition - outFragPosition);
			specDiff = pow(max(dot(FE, R), 0.0), 32);
		}

	
		

		vec3 diffuseColor =		vec3(texture(diffuseTexture, outTexC) * dotProdLightNormal * vec4(Lights[i].DiffuseColor.rgb, 1.0f)) * attenuation;
		vec3 specularColor =	vec3(vec4(Lights[i].SpecularColor.rgb, 1.0f)) * specDiff * attenuation;

		// cache diffuse texture instead of getting it over and over again
		color.rgb += (diffuseColor.rgb + specularColor.rgb);
	}
	color += vec3(ambient.rgb);	

}	