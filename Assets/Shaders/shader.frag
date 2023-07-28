#version 330

in vec4 vCol;
in vec2 TexCoord;
in vec3 Normal;			// You could add flat to this to make it a constant for the whole triangle
in vec3 FragPos;

out vec4 colour;

const int MAX_POINT_LIGHTS = 3;
const int MAX_SPOT_LIGHTS = 3;

struct Light
{
	vec3 colour;
	float ambientIntensity;
	float diffuseIntensity;
	//int enabled;
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

struct SpotLight
{
	PointLight base;
	vec3 direction;
	float edge;
};

struct Material
{
	vec3 baseColour;
	float specularIntensity;
	float shininess;
};

uniform int pointLightCount;
uniform int spotLightCount;

uniform DirectionalLight directionalLight;
uniform PointLight pointLights[MAX_POINT_LIGHTS];
uniform SpotLight spotLights[MAX_SPOT_LIGHTS];

uniform sampler2D mainTexture;
uniform Material material;

uniform vec3 eyePos;	// The position of the camera

vec4 CalcLightByDirection(Light light, vec3 direction)
{
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

vec4 CalcPointLight(PointLight pLight)
{
	// Get the direction from the fragment to the light
	vec3 direction = FragPos - pLight.position;
	float distance = length(direction);
	direction = normalize(direction);

	// Calculate the diffuse factor
	vec4 colour = CalcLightByDirection(pLight.base, direction);
	// Calculate the attenuation
	float attenuation = pLight.quadratic * distance * distance +
						pLight.linear * distance +
						pLight.constant;

	// Divide the colour by the attenuation
	return (colour / attenuation);
}

vec4 CalcSpotLight(SpotLight sLight)
{
	vec3 rayDirection = normalize(FragPos - sLight.base.position);
	float slFactor = dot(rayDirection, sLight.direction);
	
	if (slFactor > sLight.edge)
	{
		vec4 colour = CalcPointLight(sLight.base);
		
		return colour * ( 1.0f - ( 1.0f - slFactor ) * ( 1.0f / ( 1.0f - sLight.edge ) ) );		
	}

	return vec4(0, 0, 0, 0);
}

vec4 CalcPointLights()
{
	vec4 totalColour = vec4(0, 0, 0, 0);
	for(int i = 0; i < pointLightCount; i++)
	{
		totalColour += CalcPointLight(pointLights[i]);
	}
	
	return totalColour;
}

vec4 CalcSpotLights()
{
	vec4 totalColour = vec4(0, 0, 0, 0);
	for(int i = 0; i < spotLightCount; i++)
	{
		totalColour += CalcSpotLight(spotLights[i]);
	}
	
	return totalColour;
}

void main()
{
    // texture
    colour = texture(mainTexture, TexCoord) * vec4(material.baseColour, 1.0f);

	// Discard pixels that are mostly transparent
    if (colour.a < 0.1) discard;

	// Calculate the final colour based on the light
	vec4 finalColour = CalcLightByDirection(directionalLight.base, directionalLight.direction);
	finalColour += CalcPointLights();
	finalColour += CalcSpotLights();

	colour *= finalColour;
}
