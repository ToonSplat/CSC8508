#version 420 core

layout (binding = 0, offset = 0) uniform atomic_uint scoreCount1[5];
layout (binding = 1, offset = 0) uniform atomic_uint scoreCount2[5];
layout (binding = 2, offset = 0) uniform atomic_uint scoreCount3[5];

uniform sampler2D 	mapTex;

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


void main(void)
{
	float previousAlpha = texture(mapTex, IN.texCoord).a;

	//if (previousAlpha == 0.0f) {discard;}

	vec3 albedo = objectColour;

	gColour = vec4(albedo,1);


	vec3 previousColour = texture(mapTex, IN.texCoord).rgb;
	vec3 newColour = albedo;

	switch (currentAtomicTarget){
		case 0:
			if 		(previousColour == team1Colour && atomicCounter(scoreCount1[1]) != 0) atomicCounterDecrement(scoreCount1[1]);
			else if (previousColour == team2Colour && atomicCounter(scoreCount1[2]) != 0) atomicCounterDecrement(scoreCount1[2]);
			else if (previousColour == team3Colour && atomicCounter(scoreCount1[3]) != 0) atomicCounterDecrement(scoreCount1[3]);
			else if (previousColour == team4Colour && atomicCounter(scoreCount1[4]) != 0) atomicCounterDecrement(scoreCount1[4]);

			if 		(objectColour == team1Colour) atomicCounterIncrement(scoreCount1[1]);
			else if (objectColour == team2Colour) atomicCounterIncrement(scoreCount1[2]);
			else if (objectColour == team3Colour) atomicCounterIncrement(scoreCount1[3]);
			else if (objectColour == team3Colour) atomicCounterIncrement(scoreCount1[4]);

			break;
		case 1:
			if 		(previousColour == team1Colour && atomicCounter(scoreCount2[1]) != 0) atomicCounterDecrement(scoreCount2[1]);
			else if (previousColour == team2Colour && atomicCounter(scoreCount2[2]) != 0) atomicCounterDecrement(scoreCount2[2]);
			else if (previousColour == team3Colour && atomicCounter(scoreCount2[3]) != 0) atomicCounterDecrement(scoreCount2[3]);
			else if (previousColour == team4Colour && atomicCounter(scoreCount2[4]) != 0) atomicCounterDecrement(scoreCount2[4]);

			if 		(objectColour == team1Colour) atomicCounterIncrement(scoreCount2[1]);
			else if (objectColour == team2Colour) atomicCounterIncrement(scoreCount2[2]);
			else if (objectColour == team3Colour) atomicCounterIncrement(scoreCount2[3]);
			else if (objectColour == team3Colour) atomicCounterIncrement(scoreCount2[4]);

			break;
		case 2:
			if 		(previousColour == team1Colour && atomicCounter(scoreCount3[1]) != 0) atomicCounterDecrement(scoreCount3[1]);
			else if (previousColour == team2Colour && atomicCounter(scoreCount3[2]) != 0) atomicCounterDecrement(scoreCount3[2]);
			else if (previousColour == team3Colour && atomicCounter(scoreCount3[3]) != 0) atomicCounterDecrement(scoreCount3[3]);
			else if (previousColour == team4Colour && atomicCounter(scoreCount3[4]) != 0) atomicCounterDecrement(scoreCount3[4]);

			if 		(objectColour == team1Colour) atomicCounterIncrement(scoreCount3[1]);
			else if (objectColour == team2Colour) atomicCounterIncrement(scoreCount3[2]);
			else if (objectColour == team3Colour) atomicCounterIncrement(scoreCount3[3]);
			else if (objectColour == team3Colour) atomicCounterIncrement(scoreCount3[4]);

			break;

	}
}