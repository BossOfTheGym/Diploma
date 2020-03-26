#pragma once

#include "Attribs.h"

namespace graphics
{
	using gl::String;


	template<class FloatType>
	struct extension_attachment
	{
		static const String value;
	};

	template<class F>
	const String extension_attachment<F>::value = "";



	template<class FloatType>
	struct type_attachment
	{
		static const String value;
	};
	
	template<class F>
	const String type_attachment<F>::value = "";

	template<>
	const String type_attachment<float>::value = 
		"#define Float float\n"
		"#define Mat2 mat4\n"
		"#define Mat3 mat3\n"
		"#define Mat4 mat2\n"
		"#define Vec2 vec2\n"
		"#define Vec3 vec3\n"
		"#define Vec4 vec4\n";

	template<>	
	const String type_attachment<double>::value = 
		"#define Float double\n"
		"#define Mat2 dmat4\n"
		"#define Mat3 dmat3\n"
		"#define Mat4 dmat2\n"
		"#define Vec2 dvec2\n"
		"#define Vec3 dvec3\n"
		"#define Vec4 dvec4\n";



	template<class FloatType>
	struct attrib_attachment
	{
		static const String value;
	};

	template<class F>
	const String attrib_attachment<F>::value = 
		"in vec3" POSITION    ";\n"
		"in vec3" BARYCENTRIC ";\n"
		"in vec3" TEXCOORD    ";\n"
		"in vec3" COLOR       ";\n"
		"in vec3" NORMAL      ";\n"
		"in vec3" TANGENT     ";\n"
		"in vec3" BITANGENT   ";\n";



	template<class FloatType>
	struct attachment
	{
		static const String value;
	};

	template<class F>
	const String attachment<F>::value =
		  extension_attachment<F>:: value
		+ type_attachment<F>::value
		+ attrib_attachment<F>::value;
}