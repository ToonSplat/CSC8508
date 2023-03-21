#version 420 core

#define TEAM_COUNT 4

layout (binding = 0, offset = 0) uniform atomic_uint scoreCount[TEAM_COUNT];

uniform sampler2D 	mapTex;

uniform vec2 screenSize;

uniform vec3 team1Colour;
uniform vec3 team2Colour;
uniform vec3 team3Colour;
uniform vec3 team4Colour;

uniform vec3 objectColour;

in Vertex
{
	vec4 colour;
	vec4 localPos;
	vec3 worldPos;
	vec2 texCoord;
} IN;

out vec4 fragColor;

void main(void)
{
	vec2 uv = gl_FragCoord.xy / screenSize;

	vec3 previousColour = texture(mapTex, uv.xy).rgb;


	if (previousColour == vec3(1.0,1.0,1.0)) discard;
	
	fragColor = vec4(objectColour, 1.0);

	// Update the score count for the relevant team based on the previous and current color
	if 		(previousColour == team1Colour && atomicCounter(scoreCount[0]) != 0) atomicCounterDecrement(scoreCount[0]);
	else if (previousColour == team2Colour && atomicCounter(scoreCount[1]) != 0) atomicCounterDecrement(scoreCount[1]);
	else if (previousColour == team3Colour && atomicCounter(scoreCount[2]) != 0) atomicCounterDecrement(scoreCount[2]);
	else if (previousColour == team4Colour && atomicCounter(scoreCount[3]) != 0) atomicCounterDecrement(scoreCount[3]);

	if 		(objectColour == team1Colour) atomicCounterIncrement(scoreCount[0]);
	else if (objectColour == team2Colour) atomicCounterIncrement(scoreCount[1]);
	else if (objectColour == team3Colour) atomicCounterIncrement(scoreCount[2]);
	else if (objectColour == team4Colour) atomicCounterIncrement(scoreCount[3]);
	
	
}