#version 400 core
#extension GL_ARB_bindless_texture : enable

layout(bindless_sampler) uniform sampler2D bindless;
uniform textures{
    sampler2D arr[30];
} bindless_textures;

layout (std140) uniform materials{
	vec2 key[100];
} materialReferencer;

struct ImpactPoint{
	vec3 position;
	vec3 colour;
	float radius;
};
#define MAX_IMPACT_POINTS 300
uniform ImpactPoint impactPoints[MAX_IMPACT_POINTS];

uniform int impactPointCount;
uniform int materialIndex;

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

vec4 modulus(vec4 x){return x - floor(x * (1.0/289.0)) * 289.0;}
vec4 permutate(vec4 x){return modulus(((x * 34.0) + 1.0) * x);}

float SplatNoise(vec3 inWorldPos){
	vec3 floorP = floor(inWorldPos);
	vec3 dist = inWorldPos - floorP;
	dist = dist * dist * (3.0 - 2.0 * dist);

	vec4 b = floorP.xxyy + vec4(0.0, 1.0, 0.0, 1.0);
	vec4 permB = permutate(b.xyxy);
	vec4 permB2 = permutate(permB.xyxy + b.zzww);

	vec4 c = permB2 + floorP.zzzz;
	vec4 permC = permutate(c);
	vec4 permC2 = permutate(c + 1.0);

	vec4 fractalC = fract(permC * (1.0/41.0));
	vec4 fractalC2 = fract(permC2 * (1.0/41.0));

	vec4 shapePass = fractalC2 * dist.z + fractalC * (1.0 - dist.z);
	vec2 shapePass2 = shapePass.yw * dist.x + shapePass.xz * (1.0 - dist.x);

	return shapePass2.y * dist.y + shapePass2.x * (1.0 - dist.y);
}

void main(void)
{
	float shadow = 1.0; // New !
	
	if(IN.shadowProj.w > 0.0) { // New !
		shadow = textureProj(shadowTex , IN.shadowProj) * 0.7f;
	}

	vec3  incident = normalize ( lightPos - IN.worldPos );
	float lambert  = max (0.0 , dot ( incident , IN.normal )) * 0.9; 
	
	vec3 viewDir = normalize ( cameraPos - IN.worldPos );
	vec3 halfDir = normalize ( incident + viewDir );

	float rFactor = max (0.0 , dot ( halfDir , IN.normal ));
	float sFactor = pow ( rFactor , 80.0 );
	
	vec4 albedo = IN.colour;
	//if(hasTexture) {
	// albedo *= texture(mainTex, IN.texCoord);
	//}
	vec2 linkedMaterial = materialReferencer.key[materialIndex];
	//fragColor = vec4(linkedMaterial, 0, 1.0);
	//return;
	albedo *= texture(bindless_textures.arr[int(linkedMaterial.x)], IN.texCoord);

	for (int i = 0; i < impactPointCount; i++){
		float distanceBetween = distance(IN.localPos.xyz, impactPoints[i].position + objectPosition);
		float distancePercentage = distanceBetween / impactPoints[i].radius;
		if (distanceBetween <= impactPoints[i].radius - SplatNoise((IN.localPos.xyz - objectPosition)*(5+(0.1*(mod(i, 10)))))){
			albedo = vec4(impactPoints[i].colour, 1.0);
		}
	}
	
	albedo.rgb = pow(albedo.rgb, vec3(2.2));
	
	fragColor.rgb = albedo.rgb * 0.05f; //ambient
	
	fragColor.rgb += albedo.rgb * lightColour.rgb * lambert * shadow; //diffuse light
	
	fragColor.rgb += lightColour.rgb * sFactor * shadow; //specular light
	
	fragColor.rgb = pow(fragColor.rgb, vec3(1.0 / 2.2f));
	
	fragColor.a = albedo.a;
}