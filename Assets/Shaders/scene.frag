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
uniform sampler2DShadow shadowTex;

uniform vec3	lightPos;
uniform float	lightRadius;
uniform vec4	lightColour;

uniform vec3	cameraPos;

uniform vec3 objectPosition;

uniform bool hasTexture;

in Vertex
{
	vec4 colour;
	vec2 texCoord;
	vec4 shadowProj;
	vec3 normal;
	vec3 worldPos;
	vec4 localPos;
} IN;

out vec4 fragColor;

void main(void)
{
	float shadow = 1.0; // New !
	
	if( IN . shadowProj . w > 0.0) { // New !
		shadow = textureProj ( shadowTex , IN . shadowProj ) * 0.5f;
	}

	vec3  incident = normalize ( lightPos - IN.worldPos );
	float lambert  = max (0.0 , dot ( incident , IN.normal )) * 0.9; 
	
	vec3 viewDir = normalize ( cameraPos - IN.worldPos );
	vec3 halfDir = normalize ( incident + viewDir );

	float rFactor = max (0.0 , dot ( halfDir , IN.normal ));
	float sFactor = pow ( rFactor , 80.0 );
	
	vec4 albedo = IN.colour;
	
	if(hasTexture) {
	 albedo *= texture(mainTex, IN.texCoord);
	}

	
	
	
	for (int i = 0; i < impactPointCount; i++){
		//vec3 impactWorldPos = IN.worldPos - impactPoints[i].position;
		float distanceBetween = distance(IN.localPos.xyz, impactPoints[i].position + objectPosition);
		if (distanceBetween <= impactPoints[i].radius){
			albedo = vec4(impactPoints[i].colour, 1.0);
		}
	}
	
	
	albedo.rgb = pow(albedo.rgb, vec3(2.2));
	
	fragColor.rgb = albedo.rgb * 0.05f; //ambient
	
	fragColor.rgb += albedo.rgb * lightColour.rgb * lambert * shadow; //diffuse light
	
	fragColor.rgb += lightColour.rgb * sFactor * shadow; //specular light
	
	fragColor.rgb = pow(fragColor.rgb, vec3(1.0 / 2.2f));
	
	fragColor.a = albedo.a;
	
	//fragColor.rgb = objectPosition;
	//.rgb = vec3(
}