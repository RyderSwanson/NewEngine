#pragma once

//#include "main.h"
#include "Header.h"

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_glfw.h"
#include "ImGui/imgui_impl_opengl3.h"

#include "SharedVariables.h"

class GUI {
public: 
	GUI(GLFWwindow* window);
	~GUI();

	void render();

	bool show_demo_window;
	bool show_another_window;
	ImVec4 clear_color;
	ImGuiIO* io;
	GLFWwindow* window;
	int* numCollected;
};