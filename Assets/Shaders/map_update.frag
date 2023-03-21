#version 420 core

#define TEAM_COUNT 4

layout (binding = 0, offset = 0) uniform atomic_uint scoreCount[TEAM_COUNT];


layout (std140) uniform teamColours{
	vec4 teamColour[TEAM_COUNT];
};

uniform sampler2D 	mapTex;

uniform vec2 screenSize;

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

	vec4 previousColour = texture(mapTex, uv.xy);


	if (previousColour == vec4(1.0,1.0,1.0, 1.0)) discard;
	
	fragColor = vec4(objectColour, 1.0);

	for (int i = 0; i < TEAM_COUNT; i++){

		if 	(previousColour == teamColour[i] && atomicCounter(scoreCount[i]) != 0) 
			atomicCounterDecrement(scoreCount[i]);
		if 	(objectColour == teamColour[i].rgb) 
			atomicCounterIncrement(scoreCount[i]);
	}

}