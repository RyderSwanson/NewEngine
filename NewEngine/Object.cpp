#include "Object.h"

// the dontDraw variable is used to disable drawing from the ObjectManger
Object::Object(Shader* shader, Model& model, glm::vec3 location, glm::vec3 scale, int id, bool dontDraw) {
	this->shader = shader;
	this->model = &model;
	this->position = location;
	this->scale = scale;
	this->rotation = glm::vec3(0, 0, 0);
	this->id = id;
	this->dontDraw = dontDraw;
}

void Object::draw(Shader* customShader, bool sendID) {
	Shader* curShader;
	if (customShader == NULL) {
		curShader = shader;
	}
	else {
		curShader = customShader;
	}

	curShader->use();

	// send object id to shader, mainly used for screen pick shader
	if (sendID) {
		glm::ivec3 intVer = base10ToBase256(this->id);
		curShader->setIvec3("objectID", intVer);
	}

	glm::mat4 modelMat = glm::mat4(1.0f);
	modelMat = glm::translate(modelMat, position);
	modelMat = glm::scale(modelMat, this->scale);
	modelMat = glm::rotate(modelMat, this->rotation.x, glm::vec3(1, 0, 0));
	modelMat = glm::rotate(modelMat, this->rotation.y, glm::vec3(0, 1, 0));
	modelMat = glm::rotate(modelMat, this->rotation.z, glm::vec3(0, 0, 1));
	curShader->setMat4("model", modelMat);

	model->Draw(*curShader);
}

void Object::setPos(glm::vec3 pos) {
	this->position = pos;
}

void Object::setScale(glm::vec3 scale) {
	this->scale = scale;
}

glm::ivec3 Object::base10ToBase256(int base10) {
	glm::ivec3 result(0);
	int base = 256;
	int copy = base10;

	for (int i = 0; i < 3; i++) {
		if (copy < base) {
			result[i] = copy;
			copy = 0;
		}
		else {
			result[i] = copy % base;
			copy = copy / base;
		}
	}
	return result;
}