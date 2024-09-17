#version 330 core

out vec4 FragColor;

uniform ivec3 objectID;

void main()
{
	//vec3 test = vec3(float(objectID.x) / 256 * 50, float(objectID.y) / 256  * 50, float(objectID.z) / 256  * 50);
	vec3 test = vec3(float(objectID.x) / 256, float(objectID.y) / 256, float(objectID.z) / 256);
	FragColor = vec4(test, 1.0);
}