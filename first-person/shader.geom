#version 150 core

layout(lines) in;
layout(triangle_strip, max_vertices=10) out;

out float distanceScalar;

void main() 
{
	float newX;
	float newY;

	const float FOV = radians(90);
	const float FOV_SIN = sin(FOV);
	const float FOV_COS = cos(FOV);
	const float FOV_TAN = tan(FOV);

	const float FOV_ALT = radians(90)-FOV/2;
	const float FOV_ALT_SIN = sin(FOV_ALT);
	const float FOV_ALT_COS = cos(FOV_ALT);
	const float FOV_ALT_TAN = tan(FOV_ALT);

	vec4[2] vertices = vec4[2](gl_in[0].gl_Position, gl_in[1].gl_Position);

	if(vertices[0].y > vertices[0].x * FOV_ALT_TAN || vertices[1].y > vertices[1].x * FOV_ALT_TAN)
	{
		float m = (vertices[1].y-vertices[0].y)/(vertices[1].x-vertices[0].x);

		for(int i=0; i<2; i++)
		{
			if(vertices[i].x * FOV_ALT_TAN  > vertices[i].y && vertices[i].x > 0)
			{
				vertices[i].x = (FOV_ALT_COS*(m*vertices[i].x-vertices[i].y))/(FOV_ALT_COS*m-FOV_ALT_SIN);
				vertices[i].y = vertices[i].x*FOV_ALT_TAN;
			}

			else if(-vertices[i].x * FOV_ALT_TAN > vertices[i].y && vertices[i].x < 0)
			{
				vertices[i].x = (-FOV_ALT_COS*(m*vertices[i].x-vertices[i].y))/(-FOV_ALT_COS*m-FOV_ALT_SIN);
				vertices[i].y = -vertices[i].x*FOV_ALT_TAN;
			}
			
			newX = (radians(90)-atan(vertices[i].y, vertices[i].x))/(FOV/2);
			vertices[i].x = newX;

			distanceScalar = 1/sqrt(abs(vertices[i].y));
			newY = (distanceScalar)/FOV;
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