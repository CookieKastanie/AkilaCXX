#include "akila/default/layers/debug_layer.hpp"

using namespace akila;

struct Log {
	ImGuiTextBuffer buf;
	bool scrollToBottom = false;

	void clear() { buf.clear(); }

	void addLog(const char *fmt, ...) {
		va_list args;
		va_start(args, fmt);
		buf.appendfv(fmt, args);
		va_end(args);
		scrollToBottom = true;
	}

	void draw() {
		ImGui::BeginChild("GLlogs", {0, 200});
		ImGui::TextUnformatted(buf.begin());
		if(scrollToBottom)
			ImGui::SetScrollHereY(1.0f);
		scrollToBottom = false;
		ImGui::EndChild();
		if(ImGui::Button("Clear")) clear();
	}
};

static Log glLogger;

void GLAPIENTRY messageCallback(GLenum source, GLenum type, GLuint id,
	GLenum severity, GLsizei length,
	const GLchar *msg, const void *data) {
	char *_source;
	char *_type;
	char *_severity;

	switch(source) {
		case GL_DEBUG_SOURCE_API:
			_source = "API";
			break;

		case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
			_source = "WINDOW SYSTEM";
			break;

		case GL_DEBUG_SOURCE_SHADER_COMPILER:
			_source = "SHADER COMPILER";
			break;

		case GL_DEBUG_SOURCE_THIRD_PARTY:
			_source = "THIRD PARTY";
			break;

		case GL_DEBUG_SOURCE_APPLICATION:
			_source = "APPLICATION";
			break;

		case GL_DEBUG_SOURCE_OTHER:
			_source = "UNKNOWN";
			break;

		default:
			_source = "UNKNOWN";
			break;
	}

	switch(type) {
		case GL_DEBUG_TYPE_ERROR:
			_type = "ERROR";
			break;

		case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
			_type = "DEPRECATED BEHAVIOR";
			break;

		case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
			_type = "UDEFINED BEHAVIOR";
			break;

		case GL_DEBUG_TYPE_PORTABILITY:
			_type = "PORTABILITY";
			break;

		case GL_DEBUG_TYPE_PERFORMANCE:
			_type = "PERFORMANCE";
			break;

		case GL_DEBUG_TYPE_OTHER:
			_type = "OTHER";
			break;

		case GL_DEBUG_TYPE_MARKER:
			_type = "MARKER";
			break;

		default:
			_type = "UNKNOWN";
			break;
	}

	switch(severity) {
		case GL_DEBUG_SEVERITY_HIGH:
			_severity = "HIGH";
			break;

		case GL_DEBUG_SEVERITY_MEDIUM:
			_severity = "MEDIUM";
			break;

		case GL_DEBUG_SEVERITY_LOW:
			_severity = "LOW";
			break;

		case GL_DEBUG_SEVERITY_NOTIFICATION:
			_severity = "NOTIFICATION";
			return; // finalement, je ne trouve pas que la categorie
			// "notification" soit tres pertinente, et imgui en genere plein
			// donc c'est chiant
			break;

		default:
			_severity = "UNKNOWN";
			break;
	}

	printf("\n(%d) %s of %s severity, raised from %s: %s\n",
		id, _type, _severity, _source, msg);

	glLogger.addLog("\n(%d) %s of %s severity, raised from %s: %s\n",
		id, _type, _severity, _source, msg);
}



DebugLayer::DebugLayer(): show{false}, tabIndex { 0 } {
	keyListener = Signals::listen<KeyPressSignal>([&](KeyPressSignal const &s) {
		if(s.key == Inputs::Key::GRAVE_ACCENT) show = !show;
	});

	//glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageCallback(messageCallback, 0);
}

int DebugLayer::updateMeanFPS() {
	if(fpsQueue.size() == 64) fpsQueue.pop_front();
	fpsQueue.push_back(1.f / Time::delta);

	float mean = 0;
	for(auto v : fpsQueue) mean += v;

	mean /= fpsQueue.size();

	return static_cast<int>(std::round(mean));
}







void DebugLayer::guiTimings() {
	ImGui::TextColored({1., 1., 0., 1.}, "Timings:");
	std::string fps = "FPS: " + std::to_string(updateMeanFPS());
	ImGui::Text(fps.c_str());

	ImGui::Text("Timers Stats:");
	internal::TimeMetric::flushAndDrawImGui();
}

void DebugLayer::guiResources() {
	ImGui::TextColored({1., 1., 0., 1.}, "Resources:");
	ImGui::BeginTabBar("Resources");
	for(auto const &type : Resources::listing()) {
		if(ImGui::BeginTabItem(type.second.c_str())) {
			ImGui::BeginTable(("t" + type.second).c_str(), 2);

			ImGui::TableSetupColumn("Name");
			ImGui::TableSetupColumn("Refs");
			ImGui::TableHeadersRow();
			for(auto &value : Resources::listing(type.first)) {
				ImGui::TableNextRow();

				ImGui::TableSetColumnIndex(0);
				ImGui::Text(value.first.c_str());

				ImGui::TableSetColumnIndex(1);
				ImGui::Text(std::to_string(value.second.getRefCount()).c_str());
			}

			ImGui::EndTable();
			ImGui::EndTabItem();
		}
	}
	ImGui::EndTabBar();

	if(ImGui::Button("Clean all ressources")) {
		Resources::cleanAll();
	}
}

void DebugLayer::guiGLLogs() {
	ImGui::TextColored({1., 1., 0., 1.}, "GL Logs:");
	glLogger.draw();
}

void DebugLayer::guiLayers() {
	ImGui::TextColored({1., 1., 0., 1.}, "Layers:");
	for(auto const &layer : Layers::listing()) {
		std::string name = "\t- " + layer->getTypeName();
		ImGui::Text(name.c_str());
	}
}

void DebugLayer::guiECS() {
	ImGui::TextColored({1., 1., 0., 1.}, "Components:");
	for(auto const &type : ECS::componentTypeListing()) {
		std::string name = "\t- " + type.second;
		ImGui::Text(name.c_str());
	}
}

void DebugLayer::gui() {
	if(!show) return;

	ImGui::Begin("Debug");

	//ImGui::BeginTabBar("Tools");
	//if(ImGui::BeginTabItem("Timeing") {
		
	//}

	//ImGui::EndTabBar();
	
	guiTimings();
	ImGui::Separator();
	guiResources();
	ImGui::Separator();
	guiGLLogs();
	ImGui::Separator();
	guiLayers();
	ImGui::Separator();
	guiECS();
	ImGui::Separator();

	if(ImGui::Button("Restart /!\\")) Core::restart();
	if(ImGui::Button("Quit /!\\")) Window::close();

	ImGui::End();
}
