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

	enable(BLEND);
	blendFunc(SRC_ALPHA, ONE_MINUS_SRC_ALPHA);
	//enable(DEPTH_TEST);
	enable(CULL_FACE);
	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);
	enable(TEXTURE_CUBE_MAP_SEAMLESS);
}

void Renderer::prepare() {
	cameraUBO->setData(&camera->getUniforms());
	timeUBO->setData(&Time::now);
}

void Renderer::finish() {
	Material::currentUsed = -1;
}

void Renderer::useDefaultFrameBuffer() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, display->getWidth(), display->getHeight());
}

void Renderer::setSharedCamera(const std::shared_ptr<Camera> &cam) {
	camera = cam;
}

std::shared_ptr<Camera> &Renderer::getSharedCamera() {
	return camera;
}

void Renderer::affectUBOToShader(Shader *shader) {
	if(shader == nullptr) return;

	shader->setUBOIndex("akila_camera_ubo", cameraUBO->getBindingPoint());
	shader->setUBOIndex("akila_time_ubo", timeUBO->getBindingPoint());
	//shader->setUBOIndex("akila_lights_ubo", renderer->lightsUBO->getBindingPoint());
}

void Renderer::enable(Capability cap) {
	glEnable(cap);
}

void Renderer::disable(Capability cap) {
	glDisable(cap);
}

void Renderer::blendFunc(BlendFactor sfactor, BlendFactor dfactor) {
	glBlendFunc(sfactor, dfactor);
}

void Renderer::clearDepth() {
	glClear(GL_DEPTH_BUFFER_BIT);
}

void Renderer::render(const Material *material, const VAO *vao) {
	if(material->getId() != Material::currentUsed) {
		material->getShader()->bind();
		material->sendUniforms();
	}
	
	vao->draw();
}
