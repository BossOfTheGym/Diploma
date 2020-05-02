#version 450 core

in vec3 pos;

out vec4 color;

const vec3 lightPos  = vec3(0.0, 10000.0, 0.0);
const vec3 lightColor = vec3(1e8);
const vec3 ambientColor = vec3(0.005);
const vec3 diffuseColor = vec3(1.0, 0.6, 0.0);
const vec3 specColor 	= vec3(1.0, 0.6, 0.0);

uniform vec3 uEye;

void main()
{
	vec3 n = normalize(cross(dFdx(pos), dFdy(pos)));

	vec3 lightDir   = lightPos - pos;
	float lightDist = length(lightDir);
	lightDir /= lightDist;

	vec3 viewDir = normalize(uEye - pos);
	vec3 halfDir = normalize(lightDir + viewDir);

	float d = max(dot(lightDir, n), 0.0);            
	float s = 0.0;
	
	if (d > 0.0)
	{
		s = pow(max(dot(halfDir, n), 0.0), 16.0);
	}

	n = (n + vec3(1.05)) / 2;
	n = ambientColor + (d * n * lightColor + s * n * lightColor) / (lightDist * lightDist);

	n = pow(n, vec3(1.0 / 2.3));

	color = vec4(n, 1.0f);
}
