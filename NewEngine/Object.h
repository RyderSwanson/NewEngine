#pragma once

#include "Header.h"
#include "Shader.h"
#include "Model.h"

class Object{
public:
	Object(Shader* shader, Model& model, glm::vec3 location, glm::vec3 scale, int id, bool dontDraw = false);

	void draw(Shader* customShader = NULL, bool sendID = false);

	void setPos(glm::vec3 pos);
	void setScale(glm::vec3 scale);
	glm::ivec3 base10ToBase256(int base10);

	Shader* shader;
	Model* model;
	glm::vec3 position;
	glm::vec3 scale;
	glm::vec3 rotation;
	int id;
	bool dontDraw;
};