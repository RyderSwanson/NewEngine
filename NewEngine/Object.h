#pragma once

#include "Header.h"
#include "Shader.h"
#include "Model.h"

class Object{
public:
	Object(Shader* shader, Model& model, glm::vec3 location, glm::vec3 scale, int id, bool dontDraw = false);

	void draw();

	void setPos(glm::vec3 pos);
	void setScale(glm::vec3 scale);

	Shader* shader;
	Model* model;
	glm::vec3 position;
	glm::vec3 scale;
	int id;
	bool dontDraw;
};