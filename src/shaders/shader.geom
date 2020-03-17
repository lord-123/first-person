#version 420 

layout(lines) in;
layout(triangle_strip, max_vertices=256) out;

out float distanceScalar;
out vec3 texCoords;

const float RAD_90 = radians(90);

uniform float FOV = RAD_90;

void main() 
{
	float[2] screenX;
	float[2] ceilingY;
	float[2] floorY;

	float[2] heights;
	float[2] distances;

	const float FOV_ALT = RAD_90-FOV/2;
	const float FOV_ALT_TAN = tan(FOV_ALT);

	vec4[2] vertices = vec4[2](gl_in[0].gl_Position, gl_in[1].gl_Position);

	if(vertices[0].y < 0 && vertices[1].y < 0) return;
	
	if(vertices[0].y < vertices[0].x * FOV_ALT_TAN && vertices[1].y < vertices[1].x * FOV_ALT_TAN) return;
	if(vertices[0].y < -vertices[0].x * FOV_ALT_TAN && vertices[1].y < -vertices[1].x * FOV_ALT_TAN) return;

	float m = (vertices[1].y-vertices[0].y)/(vertices[1].x-vertices[0].x);

	float len = sqrt(pow(vertices[0].x-vertices[1].x,2)+pow(vertices[0].y-vertices[1].y,2));

	for(int i = 0; i < 2; i++)
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
			
		screenX[i] = (RAD_90-atan(vertices[i].y, vertices[i].x))/(FOV/2);
		
		ceilingY[i] = (RAD_90-atan(vertices[i].y,20))/(FOV/2);
		floorY[i] = (RAD_90-atan(vertices[i].y,-20))/(FOV/2);
	}

	for (int i = 0; i < 2; i++)
	{
		heights[i] = ceilingY[i] - floorY[i];
		distances[i] = sqrt(pow(vertices[i].x, 2) + pow(vertices[i].y, 2));
	}

	distanceScalar = distances[0];
	gl_Position = vec4(screenX[0], floorY[0], 0.0, 1.0);
	texCoords = vec3(0.0, 0.0, heights[0]);
	EmitVertex();

	gl_Position = vec4(screenX[0], ceilingY[0], 0.0, 1.0);
	texCoords = vec3(0.0, heights[0], heights[0]);
	EmitVertex();

	distanceScalar = distances[1];
	gl_Position = vec4(screenX[1], floorY[1], 0.0, 1.0);
	texCoords = vec3(heights[1], 0.0, heights[1]);
	EmitVertex();

	gl_Position = vec4(screenX[1], ceilingY[1], 0.0, 1.0);
	texCoords = vec3(heights[1], heights[1], heights[1]);
	EmitVertex();

	EndPrimitive();
}
