#version 420 core

struct ImpactPoint{
	vec3 position;
	vec3 colour;
	float radius;
};

struct Light{
	vec4 colour;
	vec3 position;
	float radius;
};

#define SCENE_LIGHTS 1
layout (std140) uniform lights{
	uniform Light sceneLights[SCENE_LIGHTS];
};

#define MAX_IMPACT_POINTS 300
uniform ImpactPoint impactPoints[MAX_IMPACT_POINTS];

uniform int impactPointCount;

uniform vec4 		objectColour;
layout(binding = 5) uniform sampler2D 	mainTex;
layout(binding = 6) uniform sampler2D	bumpTex;
layout(binding = 7) uniform sampler2D 	mapTex;
uniform sampler2DShadow shadowTex;

uniform vec3	cameraPos;

uniform vec3 objectPosition;

uniform bool hasTexture;

in Vertex
{
	vec4 colour;
	vec2 texCoord;
	vec4 shadowProj;
	vec3 normal;
	vec3 tangent;
    vec3 binormal;
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
		shadow = textureProj(shadowTex , IN.shadowProj);
	}

	mat3 TBN = mat3(normalize(IN.tangent), normalize(IN.binormal), normalize(IN.normal));
	
	vec3 bumpNormal;
	if(hasTexture) {
		bumpNormal = texture(bumpTex, IN.texCoord).rgb;
		bumpNormal = normalize(TBN * normalize(bumpNormal * 2.0 - 1.0));
	}
	else{
		bumpNormal = IN.normal;
	}
	vec3  incident = normalize ( sceneLights[0].position - IN.worldPos ); 
	float lambert  = max (0.0 , dot ( incident , bumpNormal )) * 0.9; 
	
	vec3 viewDir = normalize ( cameraPos - IN.worldPos );
	vec3 halfDir = normalize ( incident + viewDir );

	float rFactor = max (0.0 , dot ( halfDir , bumpNormal));
	float sFactor = pow ( rFactor , 80.0 );

	vec4 albedo = IN.colour;
	
	if(hasTexture) {
	 albedo *= texture(mainTex, IN.texCoord);
	}

	for (int i = 0; i < impactPointCount; i++){
		float distanceBetween = distance(IN.localPos.xyz, impactPoints[i].position + objectPosition);
		float distancePercentage = distanceBetween / impactPoints[i].radius;
		if (distanceBetween <= impactPoints[i].radius - SplatNoise((IN.localPos.xyz - objectPosition)*(5+(0.1*(mod(i, 10)))))){
			albedo = vec4(impactPoints[i].colour, 1.0);
		}
	}
	
	albedo.rgb = pow(albedo.rgb, vec3(2.2));
	
	fragColor.rgb = albedo.rgb * 0.05f; //ambient
	
	fragColor.rgb += albedo.rgb * sceneLights[0].colour.rgb * lambert * shadow; //diffuse light
	
	fragColor.rgb += sceneLights[0].colour.rgb * sFactor * shadow * 0.1; //specular // Change once bump-maps are introduced
	
	fragColor.rgb = pow(fragColor.rgb, vec3(1.0 / 2.2f));
	
	fragColor.a = albedo.a;
}