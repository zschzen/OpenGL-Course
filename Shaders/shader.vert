#version 330

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 tex;

out vec4 vCol;
out vec2 TexCoord;
out vec3 Normal;		// You could add flat here to make it flat shading (flat out vec3 Normal)
out vec3 FragPos;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

void main()
{
	// Transform the vertex position into world space
	vec4 WorldPos = model * vec4(pos, 1.0f);

	vCol = vec4(clamp(pos, 0.0f, 1.0f), 1.0f);			// Pass the interpolated vertex color to the fragment shader
	TexCoord = tex;										// Pass the interpolated vertex texture coordinates to the fragment shader
	Normal = mat3(transpose(inverse(model))) * normal;	// Transpose inverse matrix to transform normals correctly regardless of scale
	FragPos = WorldPos.xyz;								// Pass the fragment position to the fragment shader

	// Return the transformed and projected vertex value in clip space
	gl_Position = projection * view * WorldPos;
}