#pragma once

#include "Object.h"

// This class will handle the majority of one-off objects. Stuff like forests using one tree model will need draw on their own.
class ObjectManager {
public:
	ObjectManager(void);

	Object* createNewObject(Shader* shader, Model& model, glm::vec3 location, glm::vec3 scale, bool dontDraw = false);
	Object* createNewObject(Shader* shader, std::string modelPath, glm::vec3 location, glm::vec3 scale, bool dontDraw = false);

	void drawObjects(Shader* customShader = NULL, bool sendID = false);

	int getIDFromObject(Object object);
	Object* getObjectFromID(int id);

	std::vector<Object*>* objectList;
};