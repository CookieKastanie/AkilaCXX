#include "Akila/graphics/Renderer.hpp"
#include "Akila/core/Time.hpp"
#include <iostream>

using namespace Akila;

Renderer::Renderer(std::shared_ptr<Display> &display): display{display} {
	camera = std::make_shared<Camera>();
	cameraUBO = std::make_shared<UBO>(sizeof(camera->getUniforms()));

	timeUBO = std::make_shared<UBO>(sizeof(float));

	display->setRendererResizeCallback([this]() -> void {
		camera->onResize(this->display->getWidth(), this->display->getHeight());
	});
}

void Renderer::prepare() {
	cameraUBO->setData(&camera->getUniforms());
	timeUBO->setData(&Time::now);
}

void Renderer::useDefaultFrameBuffer() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, display->getWidth(), display->getHeight());
}

std::shared_ptr<Camera> &Renderer::getSharedCamera() {
	return camera;
}

void Renderer::loadMaterialFromFile(const std::string &fileName) {
	auto m = std::make_shared<Material>();

	m->loadFromFile(fileName);

	Shader *s = m->getShader();
	s->setUBOIndex("akila_camera_ubo", cameraUBO->getBindingPoint());
	s->setUBOIndex("akila_time_ubo", timeUBO->getBindingPoint());
	//s->setUBOIndex("akila_lights_ubo", lightsUBO->getBindingPoint());

	materials.emplace(m->getName(), m);
}

std::shared_ptr<Material> &Renderer::getMaterialbyName(const std::string &name) {
	return materials[name];
}
