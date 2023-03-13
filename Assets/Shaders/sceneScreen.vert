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

uniform vec4 		objectColour = vec4(1,1,1,1);

uniform bool hasVertexColours = false;

uniform float numberOfRows;
uniform vec2 offset;

out Vertex
{
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
	mat4 mvp 		  = (projMatrix * viewMatrix * modelMatrix);
	mat3 normalMatrix = transpose ( inverse ( mat3 ( modelMatrix )));

	OUT.shadowProj 	=  shadowMatrix * vec4 ( position,1);
	OUT.worldPos 	= ( modelMatrix * vec4 ( position ,1)). xyz ;
	OUT.localPos =  modelMatrix * vec4(position, 1.0);
	OUT.normal 		= normalize ( normalMatrix * normalize ( normal ));
	
	vec2 texCoordInv = vec2(texCoord.x, 1.0f - texCoord.y);
	OUT.texCoord	= (texCoordInv / numberOfRows) + offset;
	OUT.colour		= objectColour;

	if(hasVertexColours) {
		OUT.colour		= objectColour * colour;
	}

	vec3 wNormal = normalize(normalMatrix * normalize(normal));
    vec3 wTangent = normalize(normalMatrix * normalize(tangent.xyz));

    OUT.normal = wNormal;
    OUT.tangent = wTangent;
    OUT.binormal = cross(wTangent, wNormal) * tangent.w;

	gl_Position		= mvp * vec4(position, 1.0);
}