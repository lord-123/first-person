#version 330 core

in float distanceScalar;
out vec4 colourOut;

void main()
{
	colourOut = vec4(0.3, 0.8, 0.9, 1.0) * distanceScalar * 10;
}