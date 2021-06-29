#include "Akila/graphics/Renderer.hpp"
#include <iostream>

using namespace Akila;

Renderer::Renderer(std::shared_ptr<Display> &display): display{display} {
	camera = std::make_shared<Camera>();
	cameraUBO = std::make_shared<UBO>(sizeof(camera->getUniforms()));
}

void Renderer::prepare() {
	cameraUBO->setData(&camera->getUniforms());
}

void Renderer::useDefaultFrameBuffer() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, display->getWidth(), display->getHeight());
}

std::shared_ptr<Camera> &Renderer::getSharedCamera() {
	return camera;
}
