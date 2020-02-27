#version 420 

in float distanceScalar;
out vec4 colourOut;

void main()
{
	colourOut = vec4(vec3(0.3, 0.8, 0.9) * (500/(2*distanceScalar+250)), 1.0);
}
