#version 450 core


in VS_OUT
{
	vec2 texCoords;
} vertOut;

out vec4 outColor;


#define PASS_HORIZONTAL 0
#define PASS_VERTICAL 1

layout(binding = 0) uniform sampler2D image;
uniform int uPass;

const vec2 OFFSETS[2] =
{
	  vec2(1.0f, 0.0f)
	, vec2(0.0f, 1.0f)
};

const int SIZE = 6;
const float WEIGHTS[SIZE] = 
{
	  0.22667175022808675f
	, 0.19288374453118545f
	, 0.11884757380855239f
	, 0.05302509719336959f
	, 0.01713046245272764f
	, 0.00400731205193958f
};

void main()
{
	vec2 offset = vec2(0.0f);
	vec2 step = 1.0f / textureSize(image, 0); 
	if (uPass == PASS_HORIZONTAL || uPass == PASS_VERTICAL)
	{
		offset = OFFSETS[uPass] * step;
	}

	vec2 tex    = vertOut.texCoords;
	vec3 accum  = texture(image, tex).rgb * WEIGHTS[0];
	for (int i = 1; i < SIZE; i++)
	{
		accum += texture(image, tex + offset * i).rgb * WEIGHTS[i];
		accum += texture(image, tex - offset * i).rgb * WEIGHTS[i];
	}

	outColor = vec4(accum, 1.0f);
}