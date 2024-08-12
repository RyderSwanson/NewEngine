#pragma once

#include "Header.h"
#include "ImGui/imgui.h"

#define numAvgFrames 60

extern bool firstMouse;
extern bool pause;
extern bool debug;
extern bool fullscreen;

extern int width, height;
extern double currentFrame;
extern double deltaTime;
extern double lastFrame;
extern double frameTimeBuffer[numAvgFrames];
extern int timeSec;
extern float yaw, pitch;
extern float lastX, lastY;
extern glm::vec3 cameraDirection;
extern glm::vec3 cameraPos;
extern glm::vec3 cameraFront;
extern glm::vec3 cameraUp;
extern glm::vec3 lightPos;
extern float playerY;
extern int flashLightOn;
extern float lastflashactivation;
extern float nextTurnOff;
extern ImVec4 ambientColor;