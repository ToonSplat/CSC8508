#version 330 core

uniform mat4 modelMatrix = mat4(1.0f);
uniform mat4 viewMatrix = mat4(1.0f);
uniform mat4 projMatrix = mat4(1.0f);

in  vec3 position;
layout(location=2) in vec2 texCoord;

out Vertex {
	vec2 texCoord;
} OUT;

void main(void)		{
	mat4 mvp = projMatrix * viewMatrix * modelMatrix;
    gl_Position = mvp * vec4(position, 1.0);
    OUT.texCoord = texCoord;
}
