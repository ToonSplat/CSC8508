#version 330 core


in Vertex {
	vec2 texCoord;
    vec3 fragWorldPos;
} IN;

out vec4 fragColour;

vec4 defaultGray = vec4(0.5, 0.5, 0.5, 1.0);
vec4 team1Colour = vec4(0.0, 0.0, 1.0, 1.0);
vec4 team2Colour = vec4(0.0, 1.0, 0.0, 1.0);
vec4 team3Colour = vec4(1.0, 0.0, 0.0, 1.0);
vec4 team4Colour = vec4(0.98, 0, 0.79, 1.0);

float team1PercentageOwned = 0.2;
float team2PercentageOwned = 0.15;
float team3PercentageOwned = 0.3;
float team4PercentageOwned = 0.05;

void main(void)	{
	float team1 = -1 + (team1PercentageOwned * 2);
    float team2 = 1 - (team2PercentageOwned * 2); 
    float team3 = team1 + (team3PercentageOwned * 2);
    float team4 = team2 - (team4PercentageOwned * 2); 
    
    if(IN.fragWorldPos.x < team3 && IN.fragWorldPos.x > team1){
        fragColour = team3Colour;
    }else if(IN.fragWorldPos.x < team1){
        fragColour = team1Colour;
    }else if(IN.fragWorldPos.x > team4 && IN.fragWorldPos.x < team2){
        fragColour = team4Colour;
    }else if(IN.fragWorldPos.x > team2){
        fragColour = team2Colour;
    }else{
        fragColour = defaultGray;
    }
    
  
        
    
	//fragColour = vec4(IN.texCoord, 0, 1);
}