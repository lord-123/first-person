#version 420 

in vec3 texCoords;
uniform sampler2D img;

flat in float[2] screenX;
flat in float[2] ceilingY;
flat in float[2] floorY;

in float distanceScalar;
out vec4 colourOut;

void main()
{
	colourOut = vec4((textureProj(img, texCoords) * (500.0 / (2.0 * distanceScalar + 250.0))).xyz, 1.0);
}