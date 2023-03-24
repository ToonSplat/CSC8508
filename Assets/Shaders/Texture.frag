#version 330 core

uniform sampler2D diffuseTex;
uniform vec4 colour = vec4(1.0f, 1.0f, 1.0f, 1.0f);
uniform bool discardWhite = true;
uniform bool applyFillAmount = false;
uniform float fillAmount = 1.0f;

uniform bool useColour = false;

uniform vec4 colour;

in Vertex {
	vec2 texCoord;
} IN;

out vec4 fragColour;

void main(void)	
{
	if(applyFillAmount)
	{
		float x = IN.texCoord.x;
		if(x < 1 - fillAmount)
			discard;
	}


	if (useColour){
		fragColour = colour;
    return;
	}

	fragColour = texture(diffuseTex, IN.texCoord) * colour;
	if (fragColour.rgb == vec3(1.0, 1.0, 1.0) && discardWhite) 
		fragColour.a = 0.0f;

}