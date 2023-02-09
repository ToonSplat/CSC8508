#version 330 core


in Vertex {
	vec2 texCoord;
} IN;

out vec4 fragColour;

void main(void)	{
	//fragColour = texture(diffuseTex, IN.texCoord);
	fragColour = vec4(IN.texCoord, 0, 1);
}