#version 330 core

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;

in vec3 position;

uniform float lightRadius;
uniform vec3 lightPos;
uniform vec4 lightColour;

void main(void) {
vec3 scale = vec3(lightRadius );
vec3 worldPos = (position*scale) + lightPos;
gl_Position = (projMatrix * viewMatrix) * vec4(worldPos , 1.0);
}