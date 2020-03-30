#version 450 core

layout(points) in;
layout(triangle_strip, max_vertices = 24) out;

out OUT_GEOM
{
	vec3 normal;
	vec3 position;
	vec3 bc;
} outData;


uniform vec3 uVoxelSize;

uniform mat4  uPVM;
uniform mat4  uPV;
uniform mat4  uVM;
uniform mat4  uP;
uniform mat4  uV;
uniform mat4  uM;


vec3 triangleNormal(in vec3 v0, in vec3 v1, in vec3 v2)
{
	vec3 e10 = v1 - v0;
	vec3 e20 = v2 - v0;

	return normalize(cross(e10, e20));
}

struct VertexData
{
	vec4 worldPos;
	vec4 fullyTransformed;
};

void emitFace(in VertexData v0, in VertexData v1, in VertexData v2, in VertexData v3)
{
	vec3 normal = triangleNormal(v0.worldPos.xyz, v1.worldPos.xyz, v2.worldPos.xyz);

	gl_Position = v0.fullyTransformed;
	outData.position = v0.worldPos.xyz;
	outData.normal   = normal;
	outData.bc       = vec3(1.0f, 0.0f, 0.0f);
	EmitVertex();

	gl_Position = v1.fullyTransformed;
	outData.position = v1.worldPos.xyz;
	outData.normal   = normal;
	outData.bc       = vec3(0.0f, 1.0f, 0.0f);
	EmitVertex();

	gl_Position = v2.fullyTransformed;
	outData.position = v2.worldPos.xyz;
	outData.normal   = normal;
	outData.bc       = vec3(0.0f, 0.0f, 1.0f);
	EmitVertex();

	gl_Position = v3.fullyTransformed;
	outData.position = v3.worldPos.xyz;
	outData.normal   = normal;
	outData.bc       = vec3(1.0f, 0.0f, 0.0f);
	EmitVertex();

	EndPrimitive();
}

const vec4 clippingPlane = vec4(normalize(vec3(1.0f, 0.0f, 0.0f)), 0.0f);

void main()
{
	vec3 center = gl_in[0].gl_Position.xyz;

	if (dot(uM * vec4(center, 1.0), clippingPlane) > 0)
	{
		return;
	}

	vec3 cubeVertices[8] = 
	{
		  vec3(-1.0f, -1.0f, -1.0f)
		, vec3(-1.0f, -1.0f, +1.0f)
		, vec3(-1.0f, +1.0f, -1.0f)
		, vec3(-1.0f, +1.0f, +1.0f)
		, vec3(+1.0f, -1.0f, -1.0f)
		, vec3(+1.0f, -1.0f, +1.0f)
		, vec3(+1.0f, +1.0f, -1.0f)
		, vec3(+1.0f, +1.0f, +1.0f)
	};

	VertexData vertices[8];
	for (int i = 0; i < 8; i++)
	{
		vertices[i].worldPos         = uM * vec4(center + cubeVertices[i] * uVoxelSize / 2, 1.0f);
		vertices[i].fullyTransformed = uPV * vertices[i].worldPos;
	}

	emitFace(vertices[3], vertices[7], vertices[2], vertices[6]);
	emitFace(vertices[6], vertices[7], vertices[4], vertices[5]);
	emitFace(vertices[7], vertices[3], vertices[5], vertices[1]);
	emitFace(vertices[3], vertices[2], vertices[1], vertices[0]);
	emitFace(vertices[2], vertices[6], vertices[0], vertices[4]);
	emitFace(vertices[4], vertices[5], vertices[0], vertices[1]);
}