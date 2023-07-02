#version 330

in vec4 vCol;
in vec2 TexCoord;
in vec3 Normal;			// You could add flat to this to make it a constant for the whole triangle
in vec3 FragPos;

out vec4 colour;

struct DirectionalLight
{
	vec3 colour;
	float ambientIntensity;
	vec3 direction;
	float diffuseIntensity;
};

struct Material
{
	float specularIntensity;
	float shininess;
};

uniform sampler2D mainTexture;
uniform DirectionalLight directionalLight;
uniform Material material;

uniform vec3 eyePos;	// The position of the camera

void main()
{
	vec4 ambientColour = vec4(directionalLight.colour, 1.0f) * directionalLight.ambientIntensity;

	// Calculate diffuse lighting by taking the dot product of the normal and the light direction
	float diffuseFactor =  max(dot(normalize(Normal), normalize(directionalLight.direction)), 0.0f);
	vec4 diffuseColour = vec4(directionalLight.colour, 1.0f) * directionalLight.diffuseIntensity * diffuseFactor;

	// Calculate specular lighting
	vec4 specularColour = vec4(0.0f, 0.0f, 0.0f, 0.0f);
	if (diffuseFactor > 0.0f)
	{
		// Get the direction from the fragment to the eye
		vec3 fragToEye = normalize(eyePos - FragPos);
		// Calculate the reflection vector
		vec3 reflectedLight = normalize( reflect( normalize( directionalLight.direction ), normalize( Normal ) ) );

		// Calculate specular factor
		float specularFactor = dot(fragToEye, reflectedLight);
		if (specularFactor > 0.0f)
		{
			// Raise to the power of the material's specular power
			specularFactor = pow(specularFactor, material.shininess);
			specularColour = vec4(directionalLight.colour * material.specularIntensity * specularFactor, 1.0f);
		}
	}

	colour = texture(mainTexture, TexCoord) * (ambientColour + diffuseColour + specularColour);
}