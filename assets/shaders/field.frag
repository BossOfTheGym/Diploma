#version 450 core

in VS_OUT
{
	in vec2 texCoords;
};

out vec4 color;

layout(binding = 0) uniform sampler2D image;

const float dt = 0.003f;

void main()
{
	vec2 u;
	vec2 f;

	u = texCoords;
	
	f = u - vec2(0.5f);
	f = normalize(f);
	f = vec2(-f.y, f.x);
	
	u -= dt * f;

	color = texture(image, u);
}