#include "ObjectManager.h"

ObjectManager::ObjectManager(void) {
	objectList = new std::vector<Object*>;
	objectList->reserve(100);
}

Object* ObjectManager::createNewObject(Shader* shader, Model& model, glm::vec3 location, glm::vec3 scale, bool dontDraw) {
	Object* obj = new Object(shader, model, location, scale, (int)objectList->size(), dontDraw);
	objectList->push_back(obj);
	return obj;
}

Object* ObjectManager::createNewObject(Shader* shader, std::string modelPath, glm::vec3 location, glm::vec3 scale, bool dontDraw) {
	Model* model = new Model(modelPath);
	Object* obj = new Object(shader, *model, location, scale, (int)objectList->size() + 1, dontDraw);
	objectList->push_back(obj);
	return obj;
}

void ObjectManager::drawObjects(Shader* customShader, bool sendID) {
	int size = (int)objectList->size();
	for (int i = 0; i < size; i++) {
		if (!objectList->at(i)->dontDraw)
			objectList->at(i)->draw(customShader, sendID);
	}
}

int ObjectManager::getIDFromObject(Object object) {
	return object.id;
}

Object* ObjectManager::getObjectFromID(int id) {
	for (int i = 0; i < objectList->size(); i++) {
		if ((*objectList)[i]->id == id) {
			return (*objectList)[i];
		}
	}
}