#version 150 core

layout(lines) in;
layout(triangle_strip, max_vertices=10) out;

out float distanceScalar;

void main() 
{
	float newX;
	float newY;

	const int FOV = 90;
	const float FOV_SIN = sin(FOV);

	vec4[2] vertices = vec4[2](gl_in[0].gl_Position, gl_in[1].gl_Position);

	if(vertices[0].y > 0 || vertices[1].y > 0)
	{
		float m = (vertices[1].y-vertices[0].y)/(vertices[1].x-vertices[0].x);

		for(int i=0; i<2; i++)
		{
			if(vertices[i].x / FOV_SIN >= vertices[i].y && vertices[i].x > 0)
			{
				vertices[i].x = (FOV_SIN*(m*vertices[i].x-vertices[i].y))/(FOV_SIN*m-1);
				vertices[i].y = vertices[i].x/FOV_SIN;
			}

			if(-vertices[i].x / FOV_SIN >= vertices[i].y && vertices[i].x < 0)
			{
				vertices[i].x = (-FOV_SIN*(m*vertices[i].x-vertices[i].y))/(-FOV_SIN*m-1);
				vertices[i].y = -vertices[i].x/FOV_SIN;
			}
			
			newX = (atan(vertices[i].x, vertices[i].y)/radians(FOV))*2;
			vertices[i].x = newX;

			distanceScalar = 1/sqrt(vertices[i].y);
			newY = (distanceScalar/radians(FOV))*2;
			vertices[i].y = newY*5;

			gl_Position = vertices[i]; 
			EmitVertex();

			vertices[i].y = newY*-5;

			gl_Position = vertices[i]; 
			EmitVertex();
		}
	}

    EndPrimitive();
}