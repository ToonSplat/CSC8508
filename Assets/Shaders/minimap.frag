#version 400 core

struct ImpactPoint{
	vec3 position;
	vec3 colour;
	float radius;
};
#define MAX_IMPACT_POINTS 10
uniform ImpactPoint impactPoints[MAX_IMPACT_POINTS];

uniform int impactPointCount;

uniform vec4 		objectColour;
uniform sampler2D 	mainTex;

uniform bool hasTexture;

in Vertex
{
	vec4 colour;
	vec2 texCoord;
	vec3 worldPos;
} IN;

out vec4 fragColor;

void main(void)
{
	vec4 albedo = objectColour;

	if (impactPointCount > 0){
		for (int i = 0; i < impactPointCount; i++){
			vec3 impactWorldPos = impactPoints[i].position + IN.worldPos;
			float distanceBetween = distance(impactWorldPos, impactPoints[i].position);
			if (distanceBetween <= impactPoints[i].radius){
				albedo = vec4(impactPoints[i].colour, 1.0);
			}
		}
	}
	
	
	
	fragColor.rgb = albedo.rgb; //ambient
	
	fragColor.a = albedo.a;
}