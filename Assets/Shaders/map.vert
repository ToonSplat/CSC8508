#version 420 core

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

out Vertex
{
	vec4 colour;
	vec4 localPos;
	vec3 worldPos;
	vec2 texCoord;
} OUT;

void main(void)
{
	mat4 mvp 		  = (projMatrix * viewMatrix * modelMatrix);
	mat3 normalMatrix = transpose ( inverse ( mat3 ( modelMatrix )));

	OUT.worldPos 	= ( modelMatrix * vec4 ( position ,1)). xyz ;
	OUT.localPos =  modelMatrix * vec4(position, 1.0);
	OUT.texCoord	= texCoord;
	OUT.colour		= objectColour;

	if(hasVertexColours) {
		OUT.colour		= objectColour * colour;
	}
	gl_Position		= mvp * vec4(position, 1.0);
}