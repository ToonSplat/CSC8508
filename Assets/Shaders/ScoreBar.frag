#version 330 core

uniform float team1PercentageOwned;
uniform float team2PercentageOwned;
uniform float team3PercentageOwned;
uniform float team4PercentageOwned;

uniform vec3 defaultGray;

#define TEAM_COUNT 4
layout (std140) uniform teamColours{
	vec4 teamColour[TEAM_COUNT];
};

in Vertex {
	vec2 texCoord;
    vec3 fragWorldPos;
    vec3 position;
} IN;

out vec4 fragColour;


float sdPlane( vec2 samplePos, vec2 halfSize)
{
    vec2 edgeDistance = abs(samplePos) - halfSize;
    float outsideDist = length(max(edgeDistance, 0));
    float insideDist = min(max(edgeDistance.x, edgeDistance.y), 0);
    return outsideDist + insideDist;
}

void main(void)	{
	float team1 = -1 + (team1PercentageOwned * 2);
    float team2 = 1 - (team2PercentageOwned * 2); 
    float team3 = team1 + (team3PercentageOwned * 2);
    float team4 = team2 - (team4PercentageOwned * 2);

    float sdf = sdPlane(IN.fragWorldPos.xy, vec2(0.75, 0.5));
    
    if(IN.fragWorldPos.x < team3 && IN.fragWorldPos.x > team1){
        fragColour = teamColour[2];
    }else if(IN.fragWorldPos.x < team1){
        fragColour = teamColour[0];
    }else if(IN.fragWorldPos.x > team4 && IN.fragWorldPos.x < team2){
        fragColour = teamColour[3];
    }else if(IN.fragWorldPos.x > team2){
        fragColour = teamColour[1];
    }else{
        fragColour = vec4(defaultGray, 1.0);
    }

    if(sdf > 0.0 && sdf < 1.0){
        fragColour.a = 1 - sdf;
    }
    if(1 - sdf < 0.75){
        discard;
    }
}