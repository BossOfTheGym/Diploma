#version 450 core

layout(location = 0) in vec3 position;

uniform mat4 uPVM;

out vec3 pos;

void main()
{
	pos = position;
	gl_Position = uPVM * vec4(position, 1.0f);
}