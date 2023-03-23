#version 330 core

uniform sampler2D diffuseTex;
uniform vec4 color = vec4(1.0f);

in Vertex {
	vec2 texCoord;
} IN;

out vec4 fragColour;

void main(void)	{
	fragColour = texture(diffuseTex, IN.texCoord);
}