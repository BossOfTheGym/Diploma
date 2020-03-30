#version 450 core

in VS_OUT
{
	vec2 texCoords;
} vertOut;

out vec4 outColor;


layout(binding = 0) uniform sampler2D image;

const float gamma = 2.3;

void main()
{
	vec3 color = texture(image, vertOut.texCoords).rgb;

	// tone
	color = color / (color + vec3(1.0));
	// gamma
	color = pow(color, vec3(1.0f / gamma));

	outColor = vec4(color, 1.0f);
}