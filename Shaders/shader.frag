#version 330

in vec4 vCol;
in vec2 TexCoord;
flat in vec3 Normal;

out vec4 colour;

struct DirectionalLight
{
	vec3 colour;
	float ambientIntensity;
	vec3 direction;
	float diffuseIntensity;
};

uniform sampler2D mainTexture;
uniform DirectionalLight directionalLight;

void main()
{
	vec4 ambientColour = vec4(directionalLight.colour, 1.0f) * directionalLight.ambientIntensity;

	// Calculate diffuse lighting by taking the dot product of the normal and the light direction
	float diffuseFactor =  max(dot(normalize(Normal), normalize(directionalLight.direction)), 0.0f);
	vec4 diffuseColour = vec4(directionalLight.colour, 1.0f) * directionalLight.diffuseIntensity * diffuseFactor;
	
	colour = texture(mainTexture, TexCoord) * (ambientColour + diffuseColour);
}