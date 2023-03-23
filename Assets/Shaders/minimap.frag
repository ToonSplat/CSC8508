#version 400 core


uniform sampler2D 	mapTex;
uniform vec3 playerPosition;

uniform vec2 screenSize;

in Vertex
{
	vec4 colour;
	vec2 texCoord;
	vec3 worldPos;
	vec4 localPos;
} IN;

out vec4 fragColor;


void main(void)
{

	 //vec2 mapOffset = playerPosition.xz / screenSize; // Calculate the offset based on player position and scale

	vec2 mapPos = playerPosition.xz / screenSize;
	vec2 mapOffset = vec2(0.2);
    vec4 texColor = texture(mapTex, IN.texCoord); // Sample the texture using the offset

	if (texColor.rgb == vec3(1.0,1.0,1.0)) texColor.a = 0.0f;

    fragColor = texColor; // Output the color


}