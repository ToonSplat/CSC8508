#version 420 core

uniform vec4 		objectMinimapColour;

in Vertex
{
	vec4 colour;
	vec4 localPos;
	vec3 worldPos;
	vec2 texCoord;
} IN;

layout(location = 0) out vec4 gColour;

void main(void)
{
	gColour = objectMinimapColour;
}