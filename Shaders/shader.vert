#version 330

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 tex;

out vec4 vCol;
out vec2 TexCoord;
flat out vec3 Normal;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

void main()
{
	gl_Position = projection * view * model * vec4(pos, 1.0);
	vCol = vec4(clamp(pos, 0.0f, 1.0f), 1.0f);
	
	TexCoord = tex;

	// We need to convert the normal to world space when using a non-uniform scale matrix
	Normal = mat3(transpose(inverse(model))) * normal;
}