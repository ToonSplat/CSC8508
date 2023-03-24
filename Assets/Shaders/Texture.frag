#version 330 core

uniform sampler2D diffuseTex;
uniform vec4 colour = vec4(1.0f, 1.0f, 1.0f, 1.0f);

uniform bool useColour = false;


in Vertex {
	vec2 texCoord;
} IN;

out vec4 fragColour;

void main(void)	
{
	if (useColour){
		fragColour = colour;
		return;
	}

	fragColour = texture(diffuseTex, IN.texCoord);
	if (fragColour.rgb == vec3(1.0,1.0,1.0)) fragColour.a = 0.0f;

}