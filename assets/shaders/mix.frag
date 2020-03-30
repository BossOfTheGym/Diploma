#version 450 core

in VS_OUT
{
	in vec2 texCoords;
} vertOut;

out vec4 outColor;


layout(binding = 0) uniform sampler2D image[2];
//layout(binding = 1) uniform sampler2D image2;


void main()
{
	vec3 color1 = texture(image[0], vertOut.texCoords).rgb;
	vec3 color2 = texture(image[1], vertOut.texCoords).rgb;

	outColor = vec4(color1 + color2, 1.0f);
}