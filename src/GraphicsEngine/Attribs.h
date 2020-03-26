#pragma once

#include <glWrap/OpenGL.h>

#define POSITION "position"
#define TEXCOORD "texCoord"
#define COLOR "color"
#define BARYCENTRIC "barycentric"
#define NORMAL "normal"
#define TANGENT "tangent"
#define BITANGENT "bitangent"

namespace graphics
{
	using Name     = gl::String;
	using Location = GLint;
}