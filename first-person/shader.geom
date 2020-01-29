#version 150 core

layout(lines) in;
layout(triangle_strip, max_vertices=10) out;

void main() 
{
	vec4 pos;
	float newX;
	float newY;

	vec3 newPos[];

	for(int i=0; i<2; i++)
	{
		pos = gl_in[i].gl_Position;

		newX = (atan(pos.x, pos.y)/radians(90))*2;
		pos.x = newX;

		newY = (1/sqrt(pos.y)/90)*2;
		pos.y = newY*200;

		gl_Position = pos; 
		EmitVertex();

		pos.y = newY*-200;

		gl_Position = pos; 
		EmitVertex();
	}

    EndPrimitive();
}