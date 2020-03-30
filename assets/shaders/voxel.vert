#version 450 core

#define POSITION_LOCATION 0
#define TEXTURE_LOCATION  1

layout(location = POSITION_LOCATION) in vec4 position;

void main()
{
	gl_Position = vec4(position.xyz, 1.0f);
}