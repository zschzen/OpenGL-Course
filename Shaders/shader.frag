#version 330

in vec4 vCol;
in vec2 TexCoord;

out vec4 colour;

//uniform sampler2D mainTexture;

void main()
{
	colour = clamp(vCol, 0.0, 1.0);
	//colour = texture(mainTexture, TexCoord);
}