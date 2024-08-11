#include "Object.h"

// the dontDraw variable is used to disable drawing from the ObjectManger
Object::Object(Shader* shader, Model& model, glm::vec3 location, glm::vec3 scale, int id, bool dontDraw) {
	this->shader = shader;
	this->model = &model;
	this->position = location;
	this->scale = scale;
	this->id = id;
	this->dontDraw = dontDraw;
}

void Object::draw() {
	shader->use();

	glm::mat4 modelMat = glm::mat4(1.0f);
	modelMat = glm::translate(modelMat, position);
	modelMat = glm::scale(modelMat, this->scale);
	shader->setMat4("model", modelMat);

	model->Draw(*shader);
}

void Object::setPos(glm::vec3 pos) {
	this->position = pos;
}

void Object::setScale(glm::vec3 scale) {
	this->scale = scale;
}