#version 330

in vec4 vCol;
in vec2 TexCoord;
in vec3 Normal;			// You could add flat to this to make it a constant for the whole triangle
in vec3 FragPos;

out vec4 colour;

const int MAX_POINT_LIGHTS = 3;

struct Light
{
	vec3 colour;
	float ambientIntensity;
	float diffuseIntensity;
	int enabled;
};

struct DirectionalLight
{
	Light base;
	vec3 direction;
};

struct PointLight
{
	Light base;
	vec3 position;
	float constant;
	float linear;
	float quadratic;
};

struct Material
{
	float specularIntensity;
	float shininess;
};

uniform int pointLightCount;

uniform DirectionalLight directionalLight;
uniform PointLight pointLights[MAX_POINT_LIGHTS];

uniform sampler2D mainTexture;
uniform Material material;

uniform vec3 eyePos;	// The position of the camera

vec4 CalcLightByDirection(Light light, vec3 direction)
{
	//if (light.enabled == 0) return vec4(0, 0, 0, 0);

	vec4 ambientColour = vec4(light.colour, 1.0f) * light.ambientIntensity;

	// Calculate diffuse lighting by taking the dot product of the normal and the light direction
	float diffuseFactor =  max(dot(normalize(Normal), normalize(direction)), 0.0f);
	vec4 diffuseColour = vec4(light.colour, 1.0f) * light.diffuseIntensity * diffuseFactor;

	// Calculate specular lighting
	vec4 specularColour = vec4(0, 0, 0, 0);
	if (diffuseFactor > 0.0f)
	{
		// Get the direction from the fragment to the eye
		vec3 fragToEye = normalize(eyePos - FragPos);
		// Calculate the reflection vector
		vec3 reflectedLight = normalize( reflect( -normalize( direction ), normalize( Normal ) ) );

		// Calculate specular factor
		float specularFactor = dot(fragToEye, reflectedLight);
		if (specularFactor > 0.0f)
		{
			// Raise to the power of the material's specular power
			specularFactor = pow(specularFactor, material.shininess);
			specularColour = vec4(light.colour * material.specularIntensity * specularFactor, 1.0f);
		}
	}

	return (ambientColour + diffuseColour + specularColour);
}

vec4 CalcPointLights()
{
	vec4 totalColour = vec4(0, 0, 0, 0);
	for(int i = 0; i < pointLightCount; i++)
	{
		// Check if the light is enabled
		//if (pointLights[i].base.enabled == 0) continue;

		// Get the direction from the fragment to the light
		vec3 direction = FragPos - pointLights[i].position;
		float distance = length(direction);
		direction = normalize(direction);

		// Calculate the diffuse factor
		vec4 colour = CalcLightByDirection(pointLights[i].base, direction);
		// Calculate the attenuation
		float attenuation = pointLights[i].quadratic * distance * distance +
							pointLights[i].linear * distance +
							pointLights[i].constant;

		// Divide the colour by the attenuation
		totalColour += (colour / attenuation);
	}

	return totalColour;
}

void main()
{
	vec4 finalColour = CalcLightByDirection(directionalLight.base, directionalLight.direction);
	finalColour += CalcPointLights();
	
	colour = texture(mainTexture, TexCoord) * finalColour;
}
