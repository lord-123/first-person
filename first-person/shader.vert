#version 150 core

in vec3 position;

uniform vec2 playerPos;
uniform float playerFacing;

void main()
{
	vec3 vertex = position;

	vertex.x -= playerPos.x;
	vertex.y -= playerPos.y;

	float facing = playerFacing;

	float x = vertex.x * sin(facing) - vertex.y * cos(facing);
	float y = vertex.x * cos(facing) + vertex.y * sin(facing);

	vertex.x = x;
	vertex.y = y;

	gl_Position = vec4(vertex,1.0);
}