#version 450 core

layout(location = 0) in vec3 position;

uniform mat4 uPVM;
uniform mat4 uM;

out vec3 pos;

void main()
{
	gl_Position = uPVM * vec4(position, 1.0f); 

	vec4 pos4 = uM * vec4(position, 1.0f);
	pos = pos4.xyz / pos4.w;
}