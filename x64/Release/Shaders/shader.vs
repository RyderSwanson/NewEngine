#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

//out vec3 ourColor;
out vec2 TexCoords;
out vec3 Normal;
out vec3 FragPos;

uniform mat4 testTrans;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;


void main()
{
	mat4 model1 = testTrans * model;
	FragPos = vec3(model1 * vec4(aPos, 1.0));
	Normal = mat3(transpose(inverse(model1))) * aNormal;

	TexCoords = aTexCoords;
	gl_Position = projection * view * vec4(FragPos, 1.0f);
};