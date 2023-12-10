#include <iostream>
#include <iomanip>
#include <sstream>
#include <thread>
#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include "GL/glew.h"
#include "GLFW/glfw3.h"

std::thread Thread;

std::string getTime(int hours, int minutes, int seconds)
{

	std::ostringstream oss;
	oss << std::setfill('0') << std::setw(2) << hours << ":" << std::setw(2) << minutes << ":" << std::setw(2) << seconds;
	std::string time = oss.str();
	return time;
}

void updateTime(int &hours, int &minutes, int &seconds, bool &start)
{

	while (start)
	{
		seconds += 1;
		std::this_thread::sleep_for(std::chrono::seconds(1));

		if (seconds == 60)
		{
			minutes += 1;
			seconds = 0;
		};

		if (minutes == 60)
		{
			hours += 1;
			minutes = 0;
		};
	};
}

static void WorkHour(ImFont *font_cron, int &hours, int &minutes, int &seconds, bool &start)
{
	ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
	ImGui::SetNextWindowPos(ImVec2(0, 0));

	ImGui::Begin("Stopwatch", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

	ImGui::PushFont(font_cron);
	std::string time = getTime(hours, minutes, seconds);

	ImGui::TextWrapped("%s", time.c_str());
	ImGui::PopFont();

	std::string buttonText = start ? "Pause" : "Start";

	if (ImGui::Button(buttonText.c_str()))
	{
		start = !start;
		Thread = std::thread(updateTime, std::ref(hours), std::ref(minutes), std::ref(seconds), std::ref(start));
		Thread.detach();
	}

	ImGui::SameLine();

	if (ImGui::Button("Reset"))
	{

		hours = 0;
		minutes = 0;
		seconds = 0;
	}

	ImGui::End();
}

int main()
{

	// Setup window
	glfwSetErrorCallback([](int error, const char *description)
						 { fprintf(stderr, "Glfw Error %d: %s\n", error, description); });
	glfwInit();

	// GL 3.0 + GLSL 130
	const char *glsl_version = "#version 130";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

	// Create window with graphics context
	GLFWwindow *window = glfwCreateWindow(220, 120, "WorkHour", nullptr, nullptr);
	glfwSetWindowAttrib(window, GLFW_RESIZABLE, GLFW_FALSE);
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1); // Enable vsync

	// Initialize OpenGL loader
	glewInit();

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	// load font
	ImGuiIO &io = ImGui::GetIO();
	io.Fonts->AddFontFromFileTTF("fonts/Roboto-Medium.ttf", 18.0f);
	ImFont *font_cron = io.Fonts->AddFontFromFileTTF("fonts/Roboto-Medium.ttf", 47.0f);

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();

	// Setup Platform/Renderer bindings
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);

	// Main loop
	int display_w, display_h;
	glfwGetFramebufferSize(window, &display_w, &display_h);

	int seconds = 0;
	int minutes = 0;
	int hours = 0;

	bool start = false;

	while (!glfwWindowShouldClose(window))
	{
		// Poll and handle events (inputs, window resize, etc.)
		glfwPollEvents();

		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		// Run our Dear ImGui application
		WorkHour(font_cron, hours, minutes, seconds, start);

		// Rendering
		ImGui::Render();
		glfwGetFramebufferSize(window, &display_w, &display_h);
		glViewport(0, 0, display_w, display_h);
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		glfwSwapBuffers(window);
	}

	if (Thread.joinable())
	{
		Thread.join();
	}

	// Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(window);
	glfwTerminate();
}