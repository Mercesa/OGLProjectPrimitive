#version 450 core

layout (location = 0) in vec3 Position;
layout (location = 1) in vec2 UV;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

out vec2 outTexC;

void main()
{
	outTexC = UV;
	gl_Position = vec4(Position, 1.0f);
};