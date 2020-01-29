#version 150 core

uniform vec2 playerPos;
uniform float playerFacing;

void main()
{
	vec4 vertex = gl_Vertex;

	vertex.x -= playerPos.x;
	vertex.y -= playerPos.y;

	float facing = playerFacing;

	float x = vertex.x * sin(facing) - vertex.y * cos(facing);
	float y = vertex.x * cos(facing) + vertex.y * sin(facing);

	gl_Position = vertex;

	gl_FrontColor = gl_Color;
}