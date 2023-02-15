#version 400 core

struct ImpactPoint{
	vec3 position;
	vec3 colour;
	float radius;
};
#define MAX_IMPACT_POINTS 1000
uniform ImpactPoint impactPoints[MAX_IMPACT_POINTS];

uniform int impactPointCount;

uniform vec4 		objectColour;
uniform sampler2D 	mainTex;

uniform bool hasTexture;

uniform vec3 objectPosition;

in Vertex
{
	vec4 colour;
	vec2 texCoord;
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
	vec4 albedo = objectColour;

	for (int i = 0; i < impactPointCount; i++){
		float distanceBetween = distance(IN.localPos.xyz, impactPoints[i].position + objectPosition);
		if (distanceBetween <= impactPoints[i].radius - SplatNoise(IN.localPos.xyz)){
			albedo = vec4(impactPoints[i].colour, 1.0);
		}
	}
	
	
	
	fragColor = albedo;

}