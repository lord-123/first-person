#version 420 

layout(lines) in;
layout(triangle_strip, max_vertices=256) out;

out float distanceScalar;
out vec3 texCoords;

const float RAD_90 = radians(90);

uniform float FOV = RAD_90;
#define FOV_ALT RAD_90-FOV/2
#define FOV_ALT_TAN tan(FOV_ALT)

vec4[2] vertices;

float m;

float[2] screenX;
float[2] ceilingY;
float[2] floorY;

float[2] heights;
float[2] distances;

void clipNear(int i)
{
	vertices[i].x = (vertices[0].x - (vertices[0].y/m));
	vertices[i].y = 0;
}

void clipLeft(int i)
{
	vertices[i].x = (vertices[0].y-m*vertices[0].x)/(-FOV_ALT_TAN-m);
	vertices[i].y = -vertices[i].x*FOV_ALT_TAN;
}

void clipRight(int i)
{
	vertices[i].x = (vertices[0].y-m*vertices[0].x)/(FOV_ALT_TAN-m);
	vertices[i].y = vertices[i].x*FOV_ALT_TAN;
}

void main() 
{
	vertices = vec4[2](gl_in[0].gl_Position, gl_in[1].gl_Position);

	// ignore wall if completely out of the camera's FOV
	if(vertices[0].y < 0 && vertices[1].y < 0) return;
	if(vertices[0].y < vertices[0].x * FOV_ALT_TAN && vertices[1].y < vertices[1].x * FOV_ALT_TAN) return;
	if(vertices[0].y < -vertices[0].x * FOV_ALT_TAN && vertices[1].y < -vertices[1].x * FOV_ALT_TAN) return;

	m = (vertices[1].y-vertices[0].y)/(vertices[1].x-vertices[0].x);

	float len = length(vertices[0]-vertices[1]);

	for(int i = 0; i < 2; i++)
	{
		if (vertices[i].y < vertices[i].x * FOV_ALT_TAN)
			clipRight(i);

		if (vertices[i].y < vertices[i].x * -FOV_ALT_TAN)
			clipLeft(i);

		if(vertices[i].y < 0)
			clipNear(i);

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
	gl_Position = vec4(screenX[0], floorY[0], -1/distances[0]+1, 1.0);
	texCoords = vec3(0.0, 0.0, heights[0]);
	EmitVertex();

	gl_Position = vec4(screenX[0], ceilingY[0], -1/distances[0]+1, 1.0);
	texCoords = vec3(0.0, heights[0], heights[0]);
	EmitVertex();

	distanceScalar = distances[1];
	gl_Position = vec4(screenX[1], floorY[1], -1/distances[1]+1, 1.0);
	texCoords = vec3(heights[1] * len / 200, 0.0, heights[1]);
	EmitVertex();

	gl_Position = vec4(screenX[1], ceilingY[1], -1/distances[1]+1, 1.0);
	texCoords = vec3(heights[1] * len / 200, heights[1], heights[1]);
	EmitVertex();

	EndPrimitive();
}
