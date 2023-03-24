#version 400 core

uniform mat4 modelMatrix 	= mat4(1.0f);
uniform mat4 viewMatrix 	= mat4(1.0f);
uniform mat4 projMatrix 	= mat4(1.0f);
uniform mat4 shadowMatrix 	= mat4(1.0f);

layout(location = 0) in vec3 position;
layout(location = 1) in vec4 colour;
layout(location = 2) in vec2 texCoord;
layout(location = 3) in vec3 normal;

uniform vec4 		objectColour = vec4(1,1,1,1);

uniform bool hasVertexColours = false;

uniform vec3 playerPosition2;

uniform vec2 screenSize2;

uniform float zoomLevel = 0.4f;

out Vertex
{
	vec4 colour;
	vec2 texCoord;
	vec3 worldPos;
	vec4 localPos;
} OUT;

void main(void)
{
	mat4 mvp 		  = (projMatrix * viewMatrix * modelMatrix);

	OUT.worldPos 	= ( modelMatrix * vec4 ( position ,1)). xyz ;
	OUT.localPos =  modelMatrix * vec4(position, 1.0);
	OUT.colour		= objectColour;


	vec2 texCoordScale = vec2(zoomLevel);

	vec4 clipSpace = mvp * vec4(playerPosition2, 1.0);
	vec3 ndcSpace = clipSpace.xyz / clipSpace.w;
	vec2 screenSpacePos = normalize((ndcSpace.xz + 1.0) / 2.0);

	vec2 zoomedScreenSpacePos = screenSpacePos * texCoordScale;
	vec2 texCoordOffset = texCoord - zoomedScreenSpacePos;
   
    vec2 finalTexCoord = texCoordOffset * texCoordScale + zoomedScreenSpacePos;

	finalTexCoord.y = 1 - finalTexCoord.y;
	//finalTexCoord.x = 1 - finalTexCoord.x;

	OUT.texCoord = finalTexCoord;

	if(hasVertexColours) {
		OUT.colour		= objectColour * colour;
	}

	gl_Position		=mvp * vec4(position, 1.0);
}