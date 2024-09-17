#include "GUI.h"

GUI::GUI(GLFWwindow* window){
	this->window = window;
	//setup imgui
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	io = &ImGui::GetIO();
	(void)io;
	io->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io->ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsLight();

	// Setup Platform/Renderer backends
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330 core");

	//Imgui stuff
	show_demo_window = true;
	show_another_window = false;
	
	numCollected = NULL;
	frameBufferTexture = NULL;
}

GUI::~GUI() {
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void GUI::render() {
	//Imgui rendering
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	static bool screen_pick = false;
	static bool display_shader_debug = false;

	if (pause) {
		//make background transparent
		ImGui::SetNextWindowBgAlpha(0.35f);

		ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoSavedSettings;

		//position stuff
		const float PAD = 10.0f;
		const ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImVec2 work_pos = viewport->WorkPos; // Use work area to avoid menu-bar/task-bar, if any!
		ImVec2 work_size = viewport->WorkSize;
		ImVec2 window_pos, window_pos_pivot, window_size;
		window_size.x = work_size.x / 3;
		window_size.y = work_size.y / 2;
		window_pos.x = window_size.x + work_pos.x + PAD;
		window_pos.y = work_pos.y + PAD;
		window_pos_pivot.x = 1.0f;
		ImGui::SetNextWindowPos(window_pos, ImGuiCond_FirstUseEver, window_pos_pivot);
		ImGui::SetNextWindowSize(window_size, ImGuiCond_FirstUseEver);

		ImGui::Begin("Debug window", NULL, window_flags);
		if (ImGui::Button("Exit Game")) {
			glfwSetWindowShouldClose(window, true);
		}
		if (ImGui::Button("Reload Shaders") && reloadShaders != NULL) {
			reloadShaders();
		}
		ImGui::Separator();
		ImGui::Checkbox("debug", &debug);
		ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000.0f / io->Framerate, io->Framerate);
		if (ImGui::IsMousePosValid())
			ImGui::Text("Mouse Position: (%.1f,%.1f)", io->MousePos.x, io->MousePos.y);
		else
			ImGui::Text("Mouse Position: <invalid>");
		ImGui::Separator();
		ImGui::ColorEdit3("clear color", (float*)&ambientColor);
		ImGui::Separator();
		ImGui::Checkbox("Enable screen picking", &screen_pick);
		ImGui::Checkbox("Display shader debug", &display_shader_debug);

		ImGui::End();

		// screen pick window
		if (screen_pick) {
			ImGui::SetNextWindowBgAlpha(0.35f);

			ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoSavedSettings;

			//position stuff
			const float PAD = 10.0f;
			const ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImVec2 work_pos = viewport->WorkPos; // Use work area to avoid menu-bar/task-bar, if any!
			ImVec2 work_size = viewport->WorkSize;
			ImVec2 window_pos, window_pos_pivot, window_size;
			window_size.x = work_size.x / 3;
			window_size.y = work_size.y / 2;
			window_pos.x = window_size.x * 3 - work_pos.x - PAD;
			window_pos.y = work_pos.y + PAD;
			window_pos_pivot.x = 1.0f;
			ImGui::SetNextWindowPos(window_pos, ImGuiCond_FirstUseEver, window_pos_pivot);
			ImGui::SetNextWindowSize(window_size, ImGuiCond_FirstUseEver);

			ImGui::Begin("screen pick", &screen_pick, window_flags);

			unsigned char pixel[4];
			glBindFramebuffer(GL_FRAMEBUFFER, 1);
			glReadPixels(io->MousePos.x, work_size.y - io->MousePos.y, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, pixel);
			glBindFramebuffer(GL_FRAMEBUFFER, 0);

			static int curID = NULL;

			if (click == 1 && !ImGui::IsWindowHovered(0)) {
				curID = objectIDFromPixels(glm::ivec3(pixel[0], pixel[1], pixel[2]));
			}
			ImGui::Text("ID: %d", curID);

			Object* tmpObject;

			if (objectManager != NULL) {
				tmpObject = objectManager->getObjectFromID(curID);
				if (tmpObject != NULL) {
					ImGui::Text("Vertex path: %s", tmpObject->shader->vertexPath);
					ImGui::Text("Fragment path: %s", tmpObject->shader->fragmentPath);
					float vspeed = 0.01f;
					float pos[3] = { tmpObject->position.x, tmpObject->position.y ,tmpObject->position.z };
					ImGui::DragFloat3("Position", pos, vspeed);
					tmpObject->position = glm::vec3(pos[0], pos[1], pos[2]);

					float scale[3] = { tmpObject->scale.x, tmpObject->scale.y ,tmpObject->scale.z };
					ImGui::DragFloat3("Scale", scale, vspeed);
					tmpObject->scale = glm::vec3(scale[0], scale[1], scale[2]);

					float rot[3] = { tmpObject->rotation.x, tmpObject->rotation.y ,tmpObject->rotation.z };
					ImGui::DragFloat3("Rotation", rot, vspeed);
					tmpObject->rotation = glm::vec3(rot[0], rot[1], rot[2]);
				}
			}


			ImGui::End();
		}
	}

	//overlay
	{
		static bool* p_open = (bool*)true;
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoBackground;

		//position stuff
		const float PAD = 10.0f;
		const ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImVec2 work_pos = viewport->WorkPos; // Use work area to avoid menu-bar/task-bar, if any!
		ImVec2 work_size = viewport->WorkSize;
		ImVec2 window_pos, window_pos_pivot;
		window_pos.x = (work_pos.x + work_size.x - PAD);
		window_pos.y = (work_pos.y + PAD);
		window_pos_pivot.x = 1.0f;
		ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
		window_flags |= ImGuiWindowFlags_NoMove;

		ImGui::Begin("overlay", p_open, window_flags);

		if (numCollected != NULL) {
			ImGui::Text("Points: %d", *numCollected);
		}
		ImGui::End();
	}

	//shader debug overlay
	if (display_shader_debug){
		static bool* p_open = (bool*)true;
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoBackground;

		//position stuff
		const float PAD = 10.0f;
		const ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImVec2 work_pos = viewport->WorkPos; // Use work area to avoid menu-bar/task-bar, if any!
		ImVec2 work_size = viewport->WorkSize;
		ImVec2 window_pos, window_pos_pivot;
		window_pos.x = (work_pos.x + work_size.x - PAD);
		window_pos.y = (work_pos.y + PAD*3);
		window_pos_pivot.x = 1.0f;
		ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
		window_flags |= ImGuiWindowFlags_NoMove;

		ImGui::Begin("debug overlay", p_open, window_flags);
		ImGui::SetCursorPosX(0);
		ImGui::SetCursorPosX(window_pos.x * 0.5f);
		ImGui::Image((void*)(intptr_t)*frameBufferTexture, ImVec2(300,300), ImVec2(0, 1), ImVec2(1, 0));
		ImGui::End();
	}

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

}

int GUI::objectIDFromPixels(glm::ivec3 pixels) {
	int id = 0;
	id += pixels.x;
	id += pixels.y * 256;
	id += pixels.z * 256 * 256;
	return id;
}