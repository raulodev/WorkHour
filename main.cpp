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

	oss << std::setfill('0')
		<< std::setw(2) << hours << " : "
		<< std::setw(2) << minutes << " : "
		<< std::setw(2) << seconds;

	std::string time = oss.str();

	return time;
}

void updateTime(int &hours, int &minutes, int &seconds, bool &start)
{

	while (start)
	{
		seconds += 1;

		if (seconds == 60)
		{
			minutes += 1;
			seconds = 0;
		}

		if (minutes == 60)
		{
			hours += 1;
			minutes = 0;
		}

		std::this_thread::sleep_for(std::chrono::seconds(1));
	};
}

static void WorkHour(ImFont *font_inter_semibold, int &hours, int &minutes, int &seconds, bool &start)
{
	ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
	ImGui::SetNextWindowPos(ImVec2(0, 0));

	ImGuiWindowFlags window_flags = 0;
	window_flags |= ImGuiWindowFlags_NoTitleBar;
	window_flags |= ImGuiWindowFlags_NoMove;
	window_flags |= ImGuiWindowFlags_NoResize;
	window_flags |= ImGuiWindowFlags_NoCollapse;

	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.27f, 0.06f, 0.36f, 1.0f));
	ImGui::Begin("Stopwatch", nullptr, window_flags);

	ImGui::Dummy(ImVec2(0.0f, 15.0f));

	ImGui::SetCursorPosX((ImGui::GetWindowSize().x - ImGui::CalcTextSize("WorkHour StopWatch").x) * 0.5f);
	ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
	ImGui::Text("WorkHour StopWatch");
	ImGui::PopStyleColor();

	std::string time = getTime(hours, minutes, seconds);

	ImGui::Dummy(ImVec2(0.0f, 1.0f));

	ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
	ImGui::PushFont(font_inter_semibold);

	// timer
	ImGui::SetCursorPosX((ImGui::GetWindowSize().x - ImGui::CalcTextSize("00 : 00 : 00").x) * 0.5f);
	ImGui::TextWrapped("%s", time.c_str());
	ImGui::PopFont();
	ImGui::PopStyleColor();

	// labels
	ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
	ImGui::SetCursorPosX(83.0f);
	ImGui::SetCursorPosY(130.0f);
	ImGui::Text("Hours");
	ImGui::SameLine();
	ImGui::SetCursorPosX(223.0f);
	ImGui::Text("Minutes");
	ImGui::SameLine();
	ImGui::SetCursorPosX(373.0f);
	ImGui::Text("Seconds");
	ImGui::PopStyleColor();

	ImGui::Dummy(ImVec2(0.0f, 20.0f));

	// start buttons
	std::string buttonText = start ? "Pause" : "Start";

	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.90f, 0.49f, 0.85f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.87f, 0.56f, 0.83f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.87f, 0.56f, 0.83f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
	ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 40.0f);

	ImGui::SetCursorPosX(130.0f);
	if (ImGui::Button(buttonText.c_str(), ImVec2(100.0f, 24.0f)))
	{
		start = !start;
		Thread = std::thread(updateTime, std::ref(hours),
							 std::ref(minutes), std::ref(seconds), std::ref(start));
		Thread.detach();
	}
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
	ImGui::PopStyleVar();

	ImGui::SameLine();

	// reset buttons
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.34f, 0.26f, 0.38f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.41f, 0.34f, 0.45f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.41f, 0.34f, 0.45f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
	ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 40.0f);

	ImGui::SetCursorPosX(248.0f);
	if (ImGui::Button("Reset", ImVec2(100.0f, 24.0f)))
	{

		hours = 0;
		minutes = 0;
		seconds = 0;
	}

	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
	ImGui::PopStyleVar();

	ImGui::End();
	ImGui::PopStyleColor();
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
	GLFWwindow *window = glfwCreateWindow(452, 213, "WorkHour", nullptr, nullptr);
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
	io.Fonts->AddFontFromFileTTF("fonts/Inter-Regular.ttf", 13.5f);
	ImFont *font_inter_semibold = io.Fonts->AddFontFromFileTTF("fonts/Inter-SemiBold.ttf", 90.0f);

	// Setup Dear ImGui style
	ImGui::StyleColorsLight();

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
		WorkHour(font_inter_semibold, hours, minutes, seconds, start);

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