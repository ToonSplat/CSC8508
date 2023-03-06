#version 330 core

uniform sampler2D diffuseTex;

in Vertex {
	vec2 texCoord;
} IN;

out vec4 fragColour;

void main(void)	{
	fragColour = texture(diffuseTex, IN.texCoord);

	if (fragColour.rgb == vec3(1.0,1.0,1.0)) fragColour.a = 0.0f;
	//fragColour = vec4(IN.texCoord, 0, 1);
}