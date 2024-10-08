#pragma once

#include "Object.h"

#include "SharedVariables.h"

class Monster {
public:
	Monster(Object* monster, irrklang::ISoundEngine* engine, glm::vec3 cameraFront, glm::vec3& playerPos);

	int isLooking();
	void checkCollision();
	void move();
	void update(glm::vec3& playerPos, glm::vec3 cameraFront, double deltaTime, int flashlightOn);

	float deltaTime;
	Object* monster;
	irrklang::ISoundEngine* engine;
	irrklang::ISound* sound;
	irrklang::ISoundEffectControl* fx;
	glm::vec3 playerPos;
	glm::vec3 cameraFront;
};