#version 420 

in vec2 texCoords;
uniform sampler2D img;

in float distanceScalar;
out vec4 colourOut;

void main()
{
//	colourOut = vec4(vec3(0.3, 0.8, 0.9) * (500/(2*distanceScalar+250)), 1.0);
	colourOut = vec4((texture(img, texCoords.xy * distanceScalar) * (500/(2*distanceScalar+250))).xyz, 1.0);
}
