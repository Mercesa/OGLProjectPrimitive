#version 450 core

layout (location = 0) in vec3 Position;
layout (location = 1) in vec3 Normal;
layout (location = 2) in vec2 UV;
layout (location = 3) in vec3 Tangent;
layout (location = 4) in vec3 Btangent;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

out vec2 outTexC;
out vec3 outNormal;
out vec3 outFragPosition;

void main()
{
	outFragPosition = vec3(model*vec4(Position.x, Position.y, Position.z, 1.0f));
	outNormal = Normal;
	outTexC = UV;

	gl_Position = projection * view * vec4(outFragPosition, 1.0f);

};