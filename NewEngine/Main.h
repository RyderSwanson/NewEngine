#pragma once
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup") // This disables the console window, add check for debug if we want to show it

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



/// <summary>
/// callback for window size using glfw
/// </summary>
/// <param name="window"></param>
/// <param name="width"></param>
/// <param name="height"></param>
void glfwFrameBufferSizeCallback(GLFWwindow* window, int width, int height);

/// <summary>
/// get inputs for movement and abilities, also handles jumping physics
/// </summary>
/// <param name="window"></param>
/// <param name="shader"></param>
/// <param name="fov"></param>
void getInput(GLFWwindow* window, Shader shader, float& fov, irrklang::ISound* walking, float batteryLevel);

void defaultMovement(GLFWwindow* window, Shader shader, float& fov, irrklang::ISound* walking, float batteryLevel);

void debugMovement(GLFWwindow* window, Shader shader, float& fov, irrklang::ISound* walking, float batteryLevel);

/// <summary>
/// Load a texture into opengl (no longer used)
/// </summary>
/// <param name="filePath"></param>
/// <returns></returns>
unsigned int loadTexture(const char* filePath);

/// <summary>
/// create and set the projection matrix for a shader
/// </summary>
/// <param name="theShader"></param>
/// <param name="width"></param>
/// <param name="height"></param>
/// <param name="fov"></param>
void setProjection(Shader theShader, int width, int height, float fov);

/// <summary>
/// callback for mouse movements using glfw
/// </summary>
/// <param name="window"></param>
/// <param name="xpos"></param>
/// <param name="ypos"></param>
void mouseCallback(GLFWwindow* window, double xpos, double ypos);

/// <summary>
/// callback for keypresses using glfw
/// </summary>
/// <param name="window"></param>
/// <param name="key"></param>
/// <param name="scancode"></param>
/// <param name="action"></param>
/// <param name="mods"></param>
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

/// <summary>
/// create the glfw opengl window instance
/// </summary>
/// <param name="width"></param>
/// <param name="height"></param>
/// <param name="name"></param>
/// <param name="fullscreen"></param>
/// <returns></returns>
GLFWwindow* createWindow(int width, int height, const char* name, int fullscreen);

/// <summary>
/// calculate fps and display to console
/// </summary>
/// <param name="deltaTime"></param>
/// <param name="time"></param>
void fps(double deltaTime, double time);

/// <summary>
/// setup light parameters inside shader
/// </summary>
/// <param name="shader"></param>
void setupLights(Shader shader);

/// <summary>
/// draw trees randomly around map
/// </summary>
/// <param name="tree"></param>
/// <param name="num"></param>
void drawForest(Object& tree, std::vector<glm::vec3>& forest, std::vector<glm::vec3>& forestScale, int num);

void generateForest(std::vector<glm::vec3>& forest, std::vector<glm::vec3>& forestScale, int num);

void updateFlashLight(Shader shader, Shader guiShader, float& batteryLevel, float deltaTime);

void collision(glm::vec3& playerPos, std::vector<glm::vec3>& forestPos, int numTrees);
