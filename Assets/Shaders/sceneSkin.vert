#version 400 core

uniform mat4 modelMatrix 	= mat4(1.0f);
uniform mat4 viewMatrix 	= mat4(1.0f);
uniform mat4 projMatrix 	= mat4(1.0f);
uniform mat4 shadowMatrix 	= mat4(1.0f);

layout(location = 0) in vec3 position;
layout(location = 1) in vec4 colour;
layout(location = 2) in vec2 texCoord;
layout(location = 3) in vec3 normal;
layout(location = 4) in vec4 tangent;

layout(location = 5) in vec4 jointWeights;
layout(location = 6) in ivec4 jointIndices;

uniform mat4 joints[128];

uniform vec4 objectColour = vec4(1,1,1,1);
uniform bool hasVertexColours = false;

out Vertex {
	vec4 colour;
	vec2 texCoord;
	vec4 shadowProj;
	vec3 normal;
	vec3 tangent;
    vec3 binormal;
	vec3 worldPos;
	vec4 localPos;
} OUT;

void main(void) 
{
	mat4 mvp = projMatrix * viewMatrix * modelMatrix;
	mat3 normalMatrix = transpose ( inverse ( mat3 ( modelMatrix )));

	OUT.shadowProj 	=  shadowMatrix * vec4(position, 1.0);
	OUT.worldPos 	=  (modelMatrix * vec4(position, 1.0)).xyz;
	OUT.localPos 	=  modelMatrix * vec4(position, 1.0);
	OUT.normal 		= normalize(normalMatrix * normalize (normal));
	OUT.texCoord 	= texCoord;
	OUT.colour		= objectColour;

	if(hasVertexColours)
		OUT.colour	= objectColour * colour;

	vec4 localNormal = vec4(normal, 1.0f);
	vec4 localPos 	= vec4(position, 1.0f);
	vec4 skelPos 	= vec4(0,0,0,0);
	vec4 skelNormal = vec4(0,0,0,0);
	vec4 otherSkelPos 	= vec4(0,0,0,0);

	for(int i = 0; i < 4; ++i) {
		int   jointIndex 	= jointIndices[i];
		float jointWeight 	= jointWeights[i];

		skelPos += joints[jointIndex] * localPos * jointWeight;
		skelNormal += joints[jointIndex] * localNormal * jointWeight;
	}
	//skelPos.xyz = position.xyz;

	vec3 wNormal = normalize(normalMatrix * normalize(normal));
    vec3 wTangent = normalize(normalMatrix * normalize(tangent.xyz));

    OUT.normal = wNormal;
    OUT.tangent = wTangent;
    OUT.binormal = cross(wTangent, wNormal) * tangent.w;


	//OUT.normal = mat3(modelMatrix) * normalize(skelNormal.xyz);	
	gl_Position = mvp * vec4(skelPos.xyz, 1.0);
	//gl_Position = mvp * localPos;
}