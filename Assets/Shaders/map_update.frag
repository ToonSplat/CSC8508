#version 420 core

layout (binding = 0, offset = 0) uniform atomic_uint scoreCount1[5];
layout (binding = 1, offset = 0) uniform atomic_uint scoreCount2[5];
layout (binding = 2, offset = 0) uniform atomic_uint scoreCount3[5];

uniform sampler2D 	mapTex;


uniform mat4 viewProj 	= mat4(1.0f);

uniform vec2 screenSize;

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


out vec4 fragColor;


void main(void)
{
	/*vec4 screenPos = viewProj * vec4(IN.worldPos, 1.0);
	vec2 uv = screenPos.xy / screenPos.w;
	
	uv = normalize((uv.xy + 1.0));
	uv = uv * 0.5 + 0.5;*/

	vec2 uv = gl_FragCoord.xy / screenSize;


	//uv.x = 1 - uv.x;
	//uv.y = 1 - uv.y;	

	// Sample the previous color from mapTex
	vec3 previousColour = texture(mapTex, uv.xy).rgb;
	//if (previousColour == vec3(1.0,1.0,1.0)) previousColour = vec3(1,0,0);
	//fragColor = vec4(previousColour,1);
	//fragColor = vec4(uv, 0,1);
	//fragColor = vec4(previousColour, 1);
	

	fragColor = vec4(objectColour, 1.0);

	vec3 newColour = objectColour;

	if (previousColour == team1Colour || previousColour == team2Colour  || previousColour == team3Colour || previousColour == team4Colour){
		fragColor = vec4(1,0,0,1);
	}

	// Update the score count for the relevant team based on the previous and current color
	if 		(previousColour == team1Colour && atomicCounter(scoreCount1[1]) != 0) atomicCounterDecrement(scoreCount1[1]);
	else if (previousColour == team2Colour && atomicCounter(scoreCount1[2]) != 0) atomicCounterDecrement(scoreCount1[2]);
	else if (previousColour == team3Colour && atomicCounter(scoreCount1[3]) != 0) atomicCounterDecrement(scoreCount1[3]);
	else if (previousColour == team4Colour && atomicCounter(scoreCount1[4]) != 0) atomicCounterDecrement(scoreCount1[4]);

	if 		(objectColour == team1Colour) atomicCounterIncrement(scoreCount1[1]);
	else if (objectColour == team2Colour) atomicCounterIncrement(scoreCount1[2]);
	else if (objectColour == team3Colour) atomicCounterIncrement(scoreCount1[3]);
	else if (objectColour == team4Colour) atomicCounterIncrement(scoreCount1[4]);
	
	// // Output the current fragment's world position to the gPos output for use in the next pass
	// gPosition = vec4(1 , 0 , 0, 1.0);

	
}