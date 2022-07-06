#include "akila/window/imgui_handler.hpp"
#include "akila/window/window.hpp"
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

using namespace akila::internal;

ImGuiIO *ImGuiHandler::io;

void ImGuiHandler::init() {
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	io = &ImGui::GetIO();
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io->ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
	//io->ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
	//io.ConfigViewportsNoAutoMerge = true;
	//io.ConfigViewportsNoTaskBarIcon = true;

	ImGui::StyleColorsClassic();

	ImGuiStyle &style = ImGui::GetStyle();
	if(io->ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}


	ImGui_ImplGlfw_InitForOpenGL(Window::window, true);
	ImGui_ImplOpenGL3_Init("#version 130");

	// theme
	/*
	fastTheming(
		{236.f / 255.f, 240.f / 255.f, 241.f / 255.f},
		{41.f / 255.f, 128.f / 255.f, 185.f / 255.f},
		{57.f / 255.f, 79.f / 255.f, 105.f / 255.f},
		{44.f / 255.f, 62.f / 255.f, 80.f / 255.f},
		{33.f / 255.f, 46.f / 255.f, 60.f / 255.f}
	);
	//*/
}

void ImGuiHandler::beginFrame() {
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void ImGuiHandler::endFrame() {
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	if(io->ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
		GLFWwindow *backup_current_context = glfwGetCurrentContext();
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		glfwMakeContextCurrent(backup_current_context);
	}
}

bool ImGuiHandler::wantCaptureMouse() {
	return ImGui::GetIO().WantCaptureMouse;
}

void ImGuiHandler::terminate() {
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void ImGuiHandler::fastTheming(Vec3 textColor, Vec3 headColor, Vec3 areaColor, Vec3 bodyColor, Vec3 popsColor) {
	ImGuiStyle &style = ImGui::GetStyle();

	style.Colors[ImGuiCol_Text] = ImVec4(textColor.x, textColor.y, textColor.z, 1.00f);
	style.Colors[ImGuiCol_TextDisabled] = ImVec4(textColor.x, textColor.y, textColor.z, 0.58f);
	style.Colors[ImGuiCol_WindowBg] = ImVec4(bodyColor.x, bodyColor.y, bodyColor.z, 0.95f);
	//style.Colors[ImGuiCol_ChildWindowBg] = ImVec4(areaColor.x, areaColor.y, areaColor.z, 0.58f);
	style.Colors[ImGuiCol_Border] = ImVec4(bodyColor.x, bodyColor.y, bodyColor.z, 0.00f);
	style.Colors[ImGuiCol_BorderShadow] = ImVec4(bodyColor.x, bodyColor.y, bodyColor.z, 0.00f);
	style.Colors[ImGuiCol_FrameBg] = ImVec4(areaColor.x, areaColor.y, areaColor.z, 1.00f);
	style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(headColor.x, headColor.y, headColor.z, 0.78f);
	style.Colors[ImGuiCol_FrameBgActive] = ImVec4(headColor.x, headColor.y, headColor.z, 1.00f);
	style.Colors[ImGuiCol_TitleBg] = ImVec4(areaColor.x, areaColor.y, areaColor.z, 1.00f);
	style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(areaColor.x, areaColor.y, areaColor.z, 0.75f);
	style.Colors[ImGuiCol_TitleBgActive] = ImVec4(headColor.x, headColor.y, headColor.z, 1.00f);
	style.Colors[ImGuiCol_MenuBarBg] = ImVec4(areaColor.x, areaColor.y, areaColor.z, 0.47f);
	style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(areaColor.x, areaColor.y, areaColor.z, 1.00f);
	style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(headColor.x, headColor.y, headColor.z, 0.21f);
	style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(headColor.x, headColor.y, headColor.z, 0.78f);
	style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(headColor.x, headColor.y, headColor.z, 1.00f);
	//style.Colors[ImGuiCol_ComboBg] = ImVec4(areaColor.x, areaColor.y, areaColor.z, 1.00f);
	style.Colors[ImGuiCol_CheckMark] = ImVec4(headColor.x, headColor.y, headColor.z, 0.80f);
	style.Colors[ImGuiCol_SliderGrab] = ImVec4(headColor.x, headColor.y, headColor.z, 0.50f);
	style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(headColor.x, headColor.y, headColor.z, 1.00f);
	style.Colors[ImGuiCol_Button] = ImVec4(headColor.x, headColor.y, headColor.z, 0.50f);
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(headColor.x, headColor.y, headColor.z, 0.86f);
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(headColor.x, headColor.y, headColor.z, 1.00f);
	style.Colors[ImGuiCol_Header] = ImVec4(headColor.x, headColor.y, headColor.z, 0.76f);
	style.Colors[ImGuiCol_HeaderHovered] = ImVec4(headColor.x, headColor.y, headColor.z, 0.86f);
	style.Colors[ImGuiCol_HeaderActive] = ImVec4(headColor.x, headColor.y, headColor.z, 1.00f);
	//style.Colors[ImGuiCol_Column] = ImVec4(headColor.x, headColor.y, headColor.z, 0.32f);
	//style.Colors[ImGuiCol_ColumnHovered] = ImVec4(headColor.x, headColor.y, headColor.z, 0.78f);
	//style.Colors[ImGuiCol_ColumnActive] = ImVec4(headColor.x, headColor.y, headColor.z, 1.00f);
	style.Colors[ImGuiCol_ResizeGrip] = ImVec4(headColor.x, headColor.y, headColor.z, 0.15f);
	style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(headColor.x, headColor.y, headColor.z, 0.78f);
	style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(headColor.x, headColor.y, headColor.z, 1.00f);
	//style.Colors[ImGuiCol_CloseButton] = ImVec4(textColor.x, textColor.y, textColor.z, 0.16f);
	//style.Colors[ImGuiCol_CloseButtonHovered] = ImVec4(textColor.x, textColor.y, textColor.z, 0.39f);
	//style.Colors[ImGuiCol_CloseButtonActive] = ImVec4(textColor.x, textColor.y, textColor.z, 1.00f);
	style.Colors[ImGuiCol_PlotLines] = ImVec4(textColor.x, textColor.y, textColor.z, 0.63f);
	style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(headColor.x, headColor.y, headColor.z, 1.00f);
	style.Colors[ImGuiCol_PlotHistogram] = ImVec4(textColor.x, textColor.y, textColor.z, 0.63f);
	style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(headColor.x, headColor.y, headColor.z, 1.00f);
	style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(headColor.x, headColor.y, headColor.z, 0.43f);
	style.Colors[ImGuiCol_PopupBg] = ImVec4(popsColor.x, popsColor.y, popsColor.z, 0.92f);
	//style.Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(areaColor.x, areaColor.y, areaColor.z, 0.73f);
}
