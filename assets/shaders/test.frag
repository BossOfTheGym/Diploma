#version 450 core

in vec3 pos;
in vec3 normal;

out vec4 color;

const vec3 lightPos = vec3(0, 5, 0);
const vec3 ambientColor = vec3(0.2, 0.0, 0.0);
const vec3 diffuseColor = vec3(0.5, 0.0, 0.0);
const vec3 specColor 	= vec3(1.0, 1.0, 1.0);

void main()
{
	vec3 n = normalize(cross(dFdx(pos), dFdy(pos)));

	vec3 lightDir = normalize(lightPos - pos);

	float lambertian = max(dot(lightDir, n), 0.0);
	float specular = 0.0;

	if(lambertian > 0.0) {
		vec3 viewDir = normalize(-pos);
		vec3 halfDir = normalize(lightDir + viewDir);
		float specAngle = max(dot(halfDir, n), 0.0);
		specular = pow(specAngle, 16.0);
	}

	//color = vec4(ambientColor + lambertian * diffuseColor + specular * specColor, 1.0);

	n = (n + vec3(1.05)) / 2;
	n = (0.02 + lambertian + specular) * n;
	n = pow(n, vec3(1 / 2.3));
	color = vec4(n, 1.0f);

	//color = vec4((normal + vec3(1.1)) / 2, 1.0);
}
