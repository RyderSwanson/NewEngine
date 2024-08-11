#include "Main.h"

int main(void) {
	//test functions object
	TestFunctions tests;

	unsigned int VBO;
	unsigned int VAO;
	unsigned int lightVAO;
	unsigned int EBO;
	unsigned int vertexShader;
	unsigned int fragmentShader;
	unsigned int shaderProgram;

	float fov = 90;

	std::vector<glm::vec3> forest, forestScale;

	srand(13);

	//irrKlang
	irrklang::ISoundEngine* engine = irrklang::createIrrKlangDevice();

	if (!engine) {
		std::cout << "audio engine failed to load" << std::endl;
	}

	//stbi
	stbi_set_flip_vertically_on_load(true);

	//glfw initial settings
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//creating a window object
	GLFWwindow* window = createWindow(800, 800, "NewEngine", 0);

	//turn on glad (so we can use opengl)
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	Shader theShader("Shaders/defaultShader.vs", "Shaders/defaultShader.fs");
	Shader emissionShader("Shaders/emissionShader.vs", "Shaders/emissionShader.fs");
	Shader skyboxShader("Shaders/skybox.vs", "Shaders/skybox.fs");
	Shader guiShader("Shaders/gui.vs", "Shaders/gui.fs");
	//setting opengl viewport size
	glViewport(0, 0, 800, 800);

	//background color, should be black
	glClearColor(0.005, 0.0, 0.01, 1);

	//vsync
	glfwSwapInterval(0);

	//setup gui
	GUI imgui(window);

	//cube
	theShader.use();

	//matricies for 3d
	glm::mat4 model = glm::mat4(1.0f);
	glm::mat4 view;
	view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

	theShader.setMat4("model", model);
	theShader.setMat4("view", view);

	glfwGetFramebufferSize(window, &width, &height);
	setProjection(theShader, width, height, fov);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glEnable(GL_DEPTH_TEST);

	lastX = width / 2;
	lastY = height / 2;

	setupLights(theShader);

	theShader.setFloat("material.shininess", 32.0f);
	theShader.setVec3("viewPos", cameraPos);


	//light cube
	emissionShader.use();

	emissionShader.setMat4("model", model);
	emissionShader.setMat4("view", view);
	setProjection(emissionShader, width, height, fov);
	emissionShader.setVec3("objectColor", glm::vec3(1.0f, 0.5f, 0.31f));
	emissionShader.setVec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));
	
	//skybox shader setup
	skyboxShader.use();

	skyboxShader.setMat4("model", model);
	skyboxShader.setMat4("view", view);
	setProjection(skyboxShader, width, height, fov);

	//gui shader setup
	guiShader.use();

	guiShader.setMat4("model", model);
	guiShader.setMat4("view", view);
	setProjection(guiShader, width, height, fov);
	
	//model loading
	ObjectManager objectManager;
	Object* ico = objectManager.createNewObject(&theShader, "Assets/ico/ico.obj", glm::vec3(0, -1.01, -120), glm::vec3(1, 1, 1));
	Object* ground = objectManager.createNewObject(&theShader, "Assets/ground/ground.obj", glm::vec3(0, -2, 0), glm::vec3(1));
	Object* building = objectManager.createNewObject(&theShader, "Assets/building/building.obj", glm::vec3(-10, -2, 10), glm::vec3(1.0f));
	Object* cube = objectManager.createNewObject(&emissionShader, "Assets/cube/cube.obj", lightPos, glm::vec3(0.1f));
	Object* skybox = objectManager.createNewObject(&skyboxShader, "Assets/skybox/skybox.obj", glm::vec3(0, -2, 0), glm::vec3(500));
	Object* gui = objectManager.createNewObject(&guiShader, "Assets/gui/gui.obj", glm::vec3(0, 0, -1), glm::vec3(1));
	
	int numTrees = 800;
	Object* tree = objectManager.createNewObject(&theShader, "Assets/tree/tree.obj", glm::vec3(0, -2, 0), glm::vec3(1), true);
	generateForest(forest, forestScale, numTrees);

	//game elements
	Objective objective(&theShader, cube, engine, cameraPos);
	imgui.numCollected = &objective.numCollected;
	Monster monster(ico, engine, cameraFront, cameraPos);
	
	//audio stuff
	irrklang::ISound* walking;
	walking = engine->play2D("Sounds/walking.mp3", true, true);
	irrklang::ISound* ambient;
	ambient = engine->play2D("Sounds/ambient.mp3", true, false);

	float batteryLevel = 1;
	//main game loop
	while (!glfwWindowShouldClose(window)) {
		currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		fps(deltaTime, currentFrame);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		getInput(window, theShader, fov, walking, batteryLevel);

		collision(cameraPos, forest, numTrees);
	
		view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

		theShader.use();


		//flashlightstuff
		theShader.setVec3("flashLight.position", cameraPos);
		theShader.setVec3("flashLight.direction", cameraFront);

		//ghetto projection matrix resetting for full screen
		glfwGetFramebufferSize(window, &width, &height);
		setProjection(theShader, width, height, fov);

		theShader.setMat4("view", view);
		theShader.setVec3("viewPos", cameraPos);

		emissionShader.use();
		setProjection(emissionShader, width, height, fov);
		emissionShader.setMat4("view", view);

		//skybox
		view = glm::lookAt(glm::vec3(0, -1, 0) , glm::vec3(0,-1,0) + cameraFront, cameraUp);
		skyboxShader.use();
		setProjection(skyboxShader, width, height, fov);
		skyboxShader.setMat4("view", view);

		//obj.draw();
		objective.update(cameraPos, batteryLevel, deltaTime);
		updateFlashLight(theShader, guiShader, batteryLevel, deltaTime);
		monster.update(cameraPos, cameraFront, deltaTime, flashLightOn);

		//draw stuff
		objectManager.drawObjects();

		drawForest(*tree, forest, forestScale, numTrees);

		imgui.render();
		
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}

void glfwFrameBufferSizeCallback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void getInput(GLFWwindow* window, Shader shader, float& fov, irrklang::ISound* walking, float batteryLevel) {
	shader.use();
	
	//key detection
	if (debug) {
		debugMovement(window, shader, fov, walking, batteryLevel);
	}
	else {
		defaultMovement(window, shader, fov, walking, batteryLevel);
	}
	
}

void defaultMovement(GLFWwindow* window, Shader shader, float& fov, irrklang::ISound* walking, float batteryLevel) {
	static float yVel = 0;
	//walking
	static float cameraSpeed = 2.5f * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS ||
		glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS ||
		glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS ||
		glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		walking->setIsPaused(false);

		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
			glm::vec3 direction(cameraFront.x, 0, cameraFront.z);
			cameraPos += cameraSpeed * glm::normalize(direction);
		}
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
			glm::vec3 direction(cameraFront.x, 0, cameraFront.z);
			cameraPos -= cameraSpeed * glm::normalize(direction);
		}
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
			glm::vec3 direction(cameraFront.x, 0, cameraFront.z);
			direction = glm::normalize(glm::cross(direction, cameraUp));
			cameraPos -= cameraSpeed * direction;
		}
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
			glm::vec3 direction(cameraFront.x, 0, cameraFront.z);
			direction = glm::normalize(glm::cross(direction, cameraUp));
			cameraPos += cameraSpeed * direction;
		}

	}
	else {
		walking->setIsPaused(true);
	}

	//sprinting
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
		cameraSpeed = 4.0f * deltaTime;
		if (fov < 100) {
			fov += 40 * deltaTime;
		}
		if (fov >= 100) {
			fov = 100;
		}
	}
	else {
		cameraSpeed = 2.5f * deltaTime;
		if (fov > 90) {
			fov -= 40 * deltaTime;
		}
		if (fov <= 90) {
			fov = 90;
		}
	}

	//flashlight toggle
	static int fKeyPressed = 0;

	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS && fKeyPressed == 0 && batteryLevel > 0) {
		fKeyPressed = 1;
		if (flashLightOn) {
			flashLightOn = 0;
		}
		else {
			flashLightOn = 1;
		}
	}
	else if (glfwGetKey(window, GLFW_KEY_F) != GLFW_PRESS && fKeyPressed == 1) {
		fKeyPressed = 0;
	}

	//player jumping
	static int spacePressed = 0;
	static int inAir = 0;
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && spacePressed == 0) {
		spacePressed = 1;
		if (!inAir) {
			yVel = 5;

			inAir = 1;
		}
	}
	else if (glfwGetKey(window, GLFW_KEY_SPACE) != GLFW_PRESS && spacePressed == 1) {
		spacePressed = 0;
	}

	playerY += yVel * deltaTime;
	if (playerY <= -1) {
		yVel = 0;
		playerY = -1;

		inAir = 0;
	}
	else {
		yVel -= 15 * deltaTime;
	}
	cameraPos.y = playerY;
}

void debugMovement(GLFWwindow* window, Shader shader, float& fov, irrklang::ISound* walking, float batteryLevel) {
	//walking
	static float cameraSpeed = 4.0f * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS ||
		glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS ||
		glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS ||
		glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		walking->setIsPaused(false);

		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
			glm::vec3 direction(cameraFront.x, 0, cameraFront.z);
			cameraPos += cameraSpeed * glm::normalize(direction);
		}
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
			glm::vec3 direction(cameraFront.x, 0, cameraFront.z);
			cameraPos -= cameraSpeed * glm::normalize(direction);
		}
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
			glm::vec3 direction(cameraFront.x, 0, cameraFront.z);
			direction = glm::normalize(glm::cross(direction, cameraUp));
			cameraPos -= cameraSpeed * direction;
		}
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
			glm::vec3 direction(cameraFront.x, 0, cameraFront.z);
			direction = glm::normalize(glm::cross(direction, cameraUp));
			cameraPos += cameraSpeed * direction;
		}

	}
	else {
		walking->setIsPaused(true);
	}

	//sprinting
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
		cameraSpeed = 6.0f * deltaTime;
		if (fov < 100) {
			fov += 40 * deltaTime;
		}
		if (fov >= 100) {
			fov = 100;
		}
	}
	else {
		cameraSpeed = 4.0f * deltaTime;
		if (fov > 90) {
			fov -= 40 * deltaTime;
		}
		if (fov <= 90) {
			fov = 90;
		}
	}

	//flashlight toggle
	static int fKeyPressed = 0;

	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS && fKeyPressed == 0) {
		fKeyPressed = 1;
		if (flashLightOn) {
			flashLightOn = 0;
		}
		else {
			flashLightOn = 1;
		}
	}
	else if (glfwGetKey(window, GLFW_KEY_F) != GLFW_PRESS && fKeyPressed == 1) {
		fKeyPressed = 0;
	}

	//player flight
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
		glm::vec3 direction(0,1,0);
		cameraPos += cameraSpeed * direction;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
		glm::vec3 direction(0,1,0);
		cameraPos -= cameraSpeed * direction;
	}
}

unsigned int loadTexture(const char* filePath) {

	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, nrChannels;

	unsigned char* data = stbi_load(filePath, &width, &height, &nrChannels, 0);

	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "image failed to load: " << filePath << std::endl;
	}

	stbi_image_free(data);

	return texture;
}

void setProjection(Shader theShader, int width, int height, float fov) {
	glm::mat4 projection;
	projection = glm::perspective(glm::radians(fov), (float)width / (float)height, 0.1f, 1000.0f);

	theShader.setMat4("projection", projection);

}

void mouseCallback(GLFWwindow* window, double xpos, double ypos) {


	if (firstMouse) {
		lastX = xpos;
		lastY = ypos;
		firstMouse = 0;
	}
	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;
	const float sensitivity = 0.1f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	if (pitch > 89.0f) {
		pitch = 89.0f;
	}
	if (pitch < -89.0f) {
		pitch = -89.0f;
	}
	//std::cout << pitch << std::endl;
	//std::cout << yaw << std::endl;

	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	if (!pause) {
		cameraFront = glm::normalize(front);

	}

}

//runs once per key press
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		if (pause) {
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			pause = 0;
		}
		else {
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			pause = 1;
		}
	}

	if (key == GLFW_KEY_F11 && action == GLFW_PRESS) {
		GLFWmonitor* monitor = glfwGetWindowMonitor(window);
		//was in windowed mode
		if (monitor == NULL) {
			monitor = glfwGetPrimaryMonitor();
		}
		const GLFWvidmode* mode = glfwGetVideoMode(monitor);

		if (!fullscreen) {
			glfwSetWindowMonitor(window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
			glfwSwapInterval(1);
			fullscreen = 1;
		}
		else {
			glfwSetWindowMonitor(window, NULL, 100, 100, 800, 800, 0);
			glfwSwapInterval(0);
			fullscreen = 0;
		}
	}

}

GLFWwindow* createWindow(int width, int height, const char* name, int fullscreen) {
	GLFWwindow* window;
	

	window = glfwCreateWindow(width, height, name, NULL, NULL);

	//check if window object worked
	if (window == NULL) {
		std::cout << "Busted window" << std::endl;
		glfwTerminate();
		exit(101);
	}


	//assign window object as the one or something
	glfwMakeContextCurrent(window);

	glfwSetFramebufferSizeCallback(window, glfwFrameBufferSizeCallback);

	glfwSetCursorPosCallback(window, mouseCallback);

	glfwSetKeyCallback(window, keyCallback);

	return window;
}

void fps(double deltaTime, double time) {
	double frameFps = 1.0 / deltaTime;

	for (int i = 0; i < numAvgFrames - 1; i++) {
		frameTimeBuffer[i] = frameTimeBuffer[i + 1];
	}

	frameTimeBuffer[numAvgFrames - 1] = frameFps;

	if (timeSec != (int)time) {
		double avg = 0;
		//std::cout << std::endl;
		//std::cout << "cur: " << frameFps << std::endl;

		for (int i = 0; i < numAvgFrames; i++) {
			avg += frameTimeBuffer[i];
		}
		avg /= numAvgFrames;
		//std::cout << "avg: " << avg << std::endl;

		timeSec = (int)time;
	}
	//std::cout << "arr: ";
	//for (int i = 0; i < 5; i++) {
	//	std::cout << lastFiveFrames[i] << ", ";
	//}
	//std::cout << std::endl;

}

void setupLights(Shader shader) {

	//dirlight
	shader.setVec3("dirLight.direction", glm::normalize(glm::vec3(-0.8f, -1.0f, 0.0f)));
	shader.setVec3("dirLight.ambient", glm::vec3(0.33f));

	shader.setVec3("dirLight.diffuse", glm::vec3(0.03f, 0.03f, 0.04f));
	shader.setVec3("dirLight.specular", glm::vec3(0.05f));

	//point light(s)
	shader.setVec3("pointLights[0].position", lightPos);

	shader.setFloat("pointLights[0].constant", 1.0f);
	shader.setFloat("pointLights[0].linear", 0.09f);
	shader.setFloat("pointLights[0].quadratic", 0.032f);

	shader.setVec3("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
	shader.setVec3("pointLights[0].diffuse", glm::vec3(0.6f, 0.6f, 0.8f));
	shader.setVec3("pointLights[0].specular", glm::vec3(0.6f, 0.6f, 0.8f));
	
	//flashlight
	shader.setVec3("flashLight.ambient", 0.05f, 0.05f, 0.05f);
	shader.setVec3("flashLight.diffuse", glm::vec3(0.8f, 0.7f, 0.4f));
	shader.setVec3("flashLight.specular", glm::vec3(0.8f, 0.7f, 0.4f));

	shader.setFloat("flashLight.constant", 1.0f);
	shader.setFloat("flashLight.linear", 0.09f);
	shader.setFloat("flashLight.quadratic", 0.032f);
	
	shader.setFloat("flashLight.cutOff", glm::cos(glm::radians(25.5f)));
	shader.setFloat("flashLight.outerCutOff", glm::cos(glm::radians(30.5f)));
}	

void drawForest(Object& tree, std::vector<glm::vec3>& forest, std::vector<glm::vec3>& forestScale, int num) {
	for (int i = 0; i < num; i++) {
		tree.setPos(forest[i]);
		tree.setScale(forestScale[i]);
		tree.draw();
	}
}

void generateForest(std::vector<glm::vec3>& forest, std::vector<glm::vec3>& forestScale, int num) {
	for (int i = 0; i < num; i++) {
		glm::vec3 curPos(rand() % 10000 - 5000, -2, rand() % 10000 - 5000);
		curPos /= 100.0f;
		curPos.y = -2;
		forest.push_back(curPos);
		float width = ((rand() % 1000) / 500.0) + .5;
		float height = ((rand() % 1000) / 500.0) + .5;
		glm::vec3 randScale(width, height, width);
		forestScale.push_back(randScale);
	}
}

void updateFlashLight(Shader shader, Shader guiShader, float& batteryLevel, float deltaTime) {
	guiShader.use();
	if (debug) batteryLevel = 1;
	if (batteryLevel <= 0) {
		batteryLevel = 0;
		flashLightOn = 0;
	}
	else if (batteryLevel > 1) {
		batteryLevel = 1;
	}
	if (flashLightOn) {
		batteryLevel -= 0.08 * deltaTime;
		shader.use();
		shader.setVec3("flashLight.diffuse", glm::vec3(0.8f, 0.7f, 0.4f));
		shader.setVec3("flashLight.specular", glm::vec3(0.8f, 0.7f, 0.4f));
	}
	else if (!flashLightOn) {
		shader.use();
		shader.setVec3("flashLight.diffuse", glm::vec3(0));
		shader.setVec3("flashLight.specular", glm::vec3(0));
	}
	
	guiShader.setFloat("batteryLevel", batteryLevel);
}

void collision(glm::vec3& playerPos, std::vector<glm::vec3>& forestPos, int numTrees) {
	float radius = 0.4f;
	float xdis, ydis;
	for (int i = 0; i < numTrees; i++) {
		glm::vec2 treePos2d(forestPos[i].x, forestPos[i].z);
		glm::vec2 playerPos2d(playerPos.x, playerPos.z);

		xdis = treePos2d.x - playerPos2d.x;
		ydis = treePos2d.y - playerPos2d.y;

		if (xdis*xdis + ydis*ydis < radius*radius) { // within circle of radius radius
			glm::vec2 direction = playerPos2d - treePos2d;
			glm::vec2 moveVec = (glm::normalize(direction) * radius) - direction;

			playerPos.x += moveVec.x;
			playerPos.z += moveVec.y;
		}
	}
}