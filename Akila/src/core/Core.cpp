#include "Akila/core/Core.hpp"

using namespace Akila;

std::shared_ptr<Display> Core::display = nullptr;
std::shared_ptr<LayerManager> Core::layerManager = nullptr;
std::shared_ptr<CoroutineManager> Core::coroutines = nullptr;
std::shared_ptr<Renderer> Core::renderer = nullptr;
std::shared_ptr<ResourcePool> Core::resourcePool = nullptr;

Core::Core() {}

int Core::run(int argc, char *argv[], void (*init)(void)) {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_SAMPLES, 0);
	glfwWindowHint(GLFW_SAMPLES, 4);

	FileSystem::init();
	display = std::make_shared<Display>();

	if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cerr << "Failed to retrieve OpenGL functions" << std::endl;
		std::exit(EXIT_FAILURE);
	}

#ifdef IMGUI
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	ImGuiIO &io = ImGui::GetIO(); (void)io;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
	//io.ConfigViewportsNoAutoMerge = true;
	//io.ConfigViewportsNoTaskBarIcon = true;

	ImGui::StyleColorsClassic();

	ImGuiStyle &style = ImGui::GetStyle();
	if(io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}

	ImGui_ImplGlfw_InitForOpenGL(display->window, true);
	ImGui_ImplOpenGL3_Init("#version 130");
#endif

	glGetError(); // bruh

	Shader::funcInit();

	layerManager = std::make_shared<LayerManager>();
	coroutines = std::shared_ptr<CoroutineManager>(new CoroutineManager());
	renderer = std::make_shared<Renderer>(display);
	resourcePool = std::make_shared<ResourcePool>();

	init();

	Time::update();
	float accumulator = 0;

	while(!display->shouldClose()) {
		Time::update();
		coroutines->flushAtFrameStart();

		accumulator += Time::delta;

		display->beforePollEvent();
		glfwPollEvents();

		while(accumulator >= Time::fixedDelta) {
			layerManager->update();
			coroutines->flushAfterFixedUpdate();
			accumulator -= Time::fixedDelta;
		}

		Time::mix = accumulator / Time::fixedDelta;

		renderer->prepare();
		coroutines->flushBeforeDraw();
		layerManager->draw();
		renderer->finish();

#ifdef IMGUI
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		layerManager->drawImGui();
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		if(io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
			GLFWwindow *backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
		}
#endif

		display->swapBuffers();
	}

	layerManager.reset();

#ifdef IMGUI
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
#endif

	return EXIT_SUCCESS;
}
