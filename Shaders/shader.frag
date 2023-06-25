#version 330

in vec4 vCol;

out vec4 colour;

void main()
{
	colour = clamp(vCol, 0.0, 1.0);
}