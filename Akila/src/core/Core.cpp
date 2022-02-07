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
	ImGui_ImplGlfw_InitForOpenGL(display->window, true);
	ImGui_ImplOpenGL3_Init("#version 130");
	ImGui::StyleColorsClassic();
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
#endif

		display->swapBuffers();
	}

	return EXIT_SUCCESS;
}
