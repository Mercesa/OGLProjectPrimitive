#version 430 core

layout (location = 0) in vec3 Position;
layout (location = 1) in vec3 Normal;
layout (location = 2) in vec2 UV;
layout (location = 3) in vec3 Tangent;
layout (location = 4) in vec3 Btangent;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;


out vec3 FragPos;
out vec2 TexCoords;
out vec3 OutNormal;

void main()
{
	vec4 viewPos = view * model * vec4(Position, 1.0f);
	FragPos = viewPos.xyz;
	gl_Position = projection * viewPos;
	
	TexCoords = UV;
	mat3 normalMatrix = transpose(inverse(mat3(view*model)));
	OutNormal = normalMatrix * OutNormal;

}