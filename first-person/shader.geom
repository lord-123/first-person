#version 330 core

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

	if(vertices[0].y < 0 && vertices[1].y < 0) return;
	 
	if(vertices[0].y < vertices[0].x * FOV_ALT_TAN && vertices[1].y < vertices[1].x * FOV_ALT_TAN) return;
	if(vertices[0].y < -vertices[0].x * FOV_ALT_TAN && vertices[1].y < -vertices[1].x * FOV_ALT_TAN) return;

	float m = (vertices[1].y-vertices[0].y)/(vertices[1].x-vertices[0].x);

	for(int i=0; i<2; i++)
    {
        if(vertices[i].y < abs(vertices[i].x) * FOV_ALT_TAN)
        {
            vec2 clipRight;
			clipRight.x = (m*vertices[i].x-vertices[i].y)/(m-FOV_ALT_TAN);
			clipRight.y = clipRight.x * FOV_ALT_TAN;

			vec2 clipLeft;
			clipLeft.x = (m*vertices[i].x-vertices[i].y)/(m+FOV_ALT_TAN);
			clipLeft.y = -clipLeft.x * FOV_ALT_TAN;

			if(clipRight.x > 0 && vertices[i].x * FOV_ALT_TAN > vertices[i].y)
			{
				vertices[i] = vec4(clipRight, vertices[i].zw);
			}
			else if(clipLeft.x < 0 && -vertices[i].x * FOV_ALT_TAN > vertices[i].y)
			{
				vertices[i] = vec4(clipLeft, vertices[i].zw);
			}
		}
			
		vertices[i].x = (radians(90)-atan(vertices[i].y, vertices[i].x))/(FOV/2);

		distanceScalar = 1/sqrt(vertices[i].y);
		newY = (distanceScalar)/FOV;
		vertices[i].y = newY*5;

		gl_Position = vertices[i]; 
		EmitVertex();

		vertices[i].y = newY*-5;

		gl_Position = vertices[i]; 
		EmitVertex();
	}

	EndPrimitive();
}