#version 450 core

in VS_OUT
{
	vec2 texCoords;
} vertOut;

out	vec4 color;

layout(binding = 0) uniform sampler2D image;

void main()
{
	color = vec4(texture(image, vertOut.texCoords).rgb, 1.0f);
}