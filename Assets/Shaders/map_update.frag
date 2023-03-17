#version 420 core

layout (binding = 0, offset = 0) uniform atomic_uint scoreCount1[5];
layout (binding = 1, offset = 0) uniform atomic_uint scoreCount2[5];
layout (binding = 2, offset = 0) uniform atomic_uint scoreCount3[5];

layout(binding = 3) uniform sampler2D 	mapTex;
layout(binding = 4) uniform sampler2D worldPosTex;

uniform vec3 team1Colour;
uniform vec3 team2Colour;
uniform vec3 team3Colour;
uniform vec3 team4Colour;

uniform int currentAtomicTarget;

uniform vec3 objectColour;

in Vertex
{
	vec4 colour;
	vec4 localPos;
	vec3 worldPos;
	vec2 texCoord;
} IN;


layout(location = 0) out vec4 gColour;
layout(location = 1) out vec4 gPosition;


void main(void)
{
	// Sample the world position texture at the current fragment's texture coordinate
	vec3 worldPos = texture(worldPosTex, IN.texCoord).rgb;

	// Calculate the texture coordinate for the previous color in the mapTex texture
	vec2 mapTexCoord = worldPos.xy / vec2(1.0, -1.0) + vec2(0.5);

	// Sample the previous color from mapTex
	vec4 previousColor = texture(mapTex, mapTexCoord);

	float previousAlpha = previousColor.a;
	vec3 previousColour = previousColor.rgb;

	gColour = vec4(objectColour, 1.0);

	vec3 newColour = objectColour;

	// Update the score count for the relevant team based on the previous and current color
	if 		(previousColour == team1Colour && atomicCounter(scoreCount1[1]) != 0) atomicCounterDecrement(scoreCount1[1]);
	else if (previousColour == team2Colour && atomicCounter(scoreCount1[2]) != 0) atomicCounterDecrement(scoreCount1[2]);
	else if (previousColour == team3Colour && atomicCounter(scoreCount1[3]) != 0) atomicCounterDecrement(scoreCount1[3]);
	else if (previousColour == team4Colour && atomicCounter(scoreCount1[4]) != 0) atomicCounterDecrement(scoreCount1[4]);

	if 		(objectColour == team1Colour) atomicCounterIncrement(scoreCount1[1]);
	else if (objectColour == team2Colour) atomicCounterIncrement(scoreCount1[2]);
	else if (objectColour == team3Colour) atomicCounterIncrement(scoreCount1[3]);
	else if (objectColour == team4Colour) atomicCounterIncrement(scoreCount1[4]);
	
	// Output the current fragment's world position to the gPos output for use in the next pass
	gPosition = vec4(IN.worldPos, 1.0);

	
}