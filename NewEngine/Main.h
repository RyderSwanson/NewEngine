#pragma once
//#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup") // This disables the console window, add check for debug if we want to show it

#include "Header.h"
#include "Shader.h"
#include "Model.h"
#include "Object.h"
#include "ObjectManager.h"
#include "Objective.h"
#include "Monster.h"
#include "TestFunctions.h"
#include "GUI.h"
#include "SharedVariables.h"


// These are the initial definitions of the variables declared in SharedVariables.h
bool firstMouse = 1;
bool pause = 0;
bool debug = 0;
bool fullscreen = 0;
int width, height;
double currentFrame;
double deltaTime = 0.0f;
double lastFrame = 0.0f;
double frameTimeBuffer[numAvgFrames] = { 0 };
int timeSec = 0;
float yaw = -90.0f, pitch = 0.0f;
float lastX, lastY;
glm::vec3 cameraDirection = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec3 lightPos = glm::vec3(0, -1, -4);
float playerY = -1;
int flashLightOn = 1;
float lastflashactivation = 0;
float nextTurnOff = 5;
ImVec4 ambientColor = ImVec4(0.1f, 0.1f, 0.1f, 1.0f);
std::vector<Shader*>* shaderList = new std::vector<Shader*>;
float fov = 90;
Shader* theShader;
Shader* emissionShader;
Shader* skyboxShader;
Shader* guiShader;
Shader* screenPickShader;


void glfwFrameBufferSizeCallback(GLFWwindow* window, int width, int height);

void getInput(GLFWwindow* window, Shader shader, float& fov, irrklang::ISound* walking, float batteryLevel, GUI* gui);

void defaultMovement(GLFWwindow* window, Shader shader, float& fov, irrklang::ISound* walking, float batteryLevel);

void debugMovement(GLFWwindow* window, Shader shader, float& fov, irrklang::ISound* walking, float batteryLevel);

unsigned int loadTexture(const char* filePath);

void setProjection(Shader shader, int width, int height, float fov);

void mouseCallback(GLFWwindow* window, double xpos, double ypos);

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

GLFWwindow* createWindow(int width, int height, const char* name, int fullscreen);

void setupLights(Shader shader);

void drawForest(Object& tree, std::vector<glm::vec3>& forest, std::vector<glm::vec3>& forestScale, int num);

void generateForest(std::vector<glm::vec3>& forest, std::vector<glm::vec3>& forestScale, int num);

void updateFlashLight(Shader shader, Shader guiShader, float& batteryLevel, float deltaTime);

void collision(glm::vec3& playerPos, std::vector<glm::vec3>& forestPos, int numTrees);

void setupFrameBuffer(GLuint& framebuffer, GLuint& colorBuffer, GLuint& rbo);

void reloadShaders();

void setupShaderUniforms();

void mouseClicks(GLFWwindow* window, GUI* gui);
