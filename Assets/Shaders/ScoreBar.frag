#version 330 core

uniform float team1PercentageOwned;
uniform float team2PercentageOwned;
uniform float team3PercentageOwned;
uniform float team4PercentageOwned;

in Vertex {
	vec2 texCoord;
    vec3 fragWorldPos;
    vec3 position;
} IN;

out vec4 fragColour;

vec3 defaultGray = vec3(0.5, 0.5, 0.5);
vec3 team1Colour = vec3(0.0, 0.0, 0.9);
vec3 team2Colour = vec3(0.0, 0.9, 0.0);
vec3 team3Colour = vec3(0.9, 0.0, 0.0);
vec3 team4Colour = vec3(0.98, 0, 0.79);

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

    float sdf = sdPlane(IN.fragWorldPos.xy, vec2(0.7, 0.5));//vec2(0.6, 0.5));
    
    if(IN.fragWorldPos.x < team3 && IN.fragWorldPos.x > team1){
        fragColour = vec4(team3Colour, 1.0);
    }else if(IN.fragWorldPos.x < team1){
        fragColour = vec4(team1Colour, 1.0);
    }else if(IN.fragWorldPos.x > team4 && IN.fragWorldPos.x < team2){
        fragColour = vec4(team4Colour, 1.0);
    }else if(IN.fragWorldPos.x > team2){
        fragColour = vec4(team2Colour, 1.0);
    }else{
        fragColour = vec4(defaultGray, 1.0);
    }

    if(sdf > 0.0 && sdf < 1.0){
        fragColour.a = 1 - sdf;
    }
    if(1 - sdf < 0.75){// 0.6
        discard;
    }
    //float sdf = sdPlane(IN.fragWorldPos.xy, vec2(0.8, 0.8));
    //fragColour = vec4(sdf, sdf, sdf, 1.0);
}