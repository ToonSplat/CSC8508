#version 420 core

layout (binding = 0, offset = 0) uniform atomic_uint scoreCount1[5];
layout (binding = 1, offset = 0) uniform atomic_uint scoreCount2[5];
layout (binding = 2, offset = 0) uniform atomic_uint scoreCount3[5];


struct ImpactPoint{
	vec3 position;
	vec3 colour;
	float radius;
};
#define MAX_IMPACT_POINTS 300
uniform ImpactPoint impactPoints[MAX_IMPACT_POINTS];

uniform int impactPointCount;

uniform vec4 		objectColour;
uniform sampler2D 	mainTex;

uniform vec3 team1Colour;
uniform vec3 team2Colour;
uniform vec3 team3Colour;
uniform vec3 team4Colour;

uniform bool hasTexture;

uniform vec3 objectPosition;

uniform bool isFloor;

uniform int currentAtomicTarget;

in Vertex
{
	vec4 colour;
	vec2 texCoord;
	vec3 worldPos;
	vec4 localPos;
} IN;

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

layout(location = 0) out vec4 gColour;
layout(location = 1) out vec4 gScore;

void main(void)
{
	vec4 albedo = objectColour;

	for (int i = 0; i < impactPointCount; i++){
		float distanceBetween = distance(IN.localPos.xyz, impactPoints[i].position + objectPosition);
		if (distanceBetween <= impactPoints[i].radius - SplatNoise((IN.localPos.xyz - objectPosition)*(5+(0.1*(mod(i, 10)))))){
			albedo = vec4(impactPoints[i].colour, 1.0);
		}
	}
	
	gColour = albedo;
	
	if (!isFloor) return;
	
	gScore = albedo;

	if (currentAtomicTarget == 0)
	{
		atomicCounterIncrement(scoreCount1[0]);
		if (albedo.rgb == team1Colour) atomicCounterIncrement(scoreCount1[1]);
		if (albedo.rgb == team2Colour) atomicCounterIncrement(scoreCount1[2]);
		if (albedo.rgb == team3Colour) atomicCounterIncrement(scoreCount1[3]);
		if (albedo.rgb == team4Colour) atomicCounterIncrement(scoreCount1[4]);

	}
	else if (currentAtomicTarget == 1)
	{
		atomicCounterIncrement(scoreCount2[0]);
		if (albedo.rgb == team1Colour) atomicCounterIncrement(scoreCount2[1]);
		if (albedo.rgb == team2Colour) atomicCounterIncrement(scoreCount2[2]);
		if (albedo.rgb == team3Colour) atomicCounterIncrement(scoreCount2[3]);
		if (albedo.rgb == team4Colour) atomicCounterIncrement(scoreCount2[4]);
	}
	else
	{
		atomicCounterIncrement(scoreCount3[0]);
		if (albedo.rgb == team1Colour) atomicCounterIncrement(scoreCount3[1]);
		if (albedo.rgb == team2Colour) atomicCounterIncrement(scoreCount3[2]);
		if (albedo.rgb == team3Colour) atomicCounterIncrement(scoreCount3[3]);
		if (albedo.rgb == team4Colour) atomicCounterIncrement(scoreCount3[4]);

	}
}