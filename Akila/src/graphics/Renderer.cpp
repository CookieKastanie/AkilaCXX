#include "Akila/graphics/Renderer.hpp"
#include "Akila/core/Time.hpp"
#include <iostream>

using namespace Akila;

Renderer::Renderer(Ptr<Display> &display): display{display} {
	camera = createPtr<Camera>();
	cameraUBO = createPtr<UBO>(sizeof(camera->getUniforms()));

	timeUBO = createPtr<UBO>(sizeof(float));

	display->setRendererResizeCallback([this]() -> void {
		camera->onResize(this->display->getWidth(), this->display->getHeight());
	});

	enable(Capability::BLEND);
	blendFunc(BlendFactor::SRC_ALPHA, BlendFactor::ONE_MINUS_SRC_ALPHA);
	enable(Capability::DEPTH_TEST);
	enable(Capability::CULL_FACE);
	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);
	enable(Capability::TEXTURE_CUBE_MAP_SEAMLESS);
}

void Renderer::prepare() {
	camera->onPrepare();

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

void Renderer::setCamera(const Ptr<Camera> &cam) {
	camera = cam;
	camera->onResize(display->getWidth(), display->getHeight());
}

Ptr<Camera> &Renderer::getCamera() {
	return camera;
}

void Renderer::affectUBOToShader(Shader *shader) {
	if(shader == nullptr) return;

	shader->setUBOIndex("akila_camera_ubo", cameraUBO->getBindingPoint());
	shader->setUBOIndex("akila_time_ubo", timeUBO->getBindingPoint());
	//shader->setUBOIndex("akila_lights_ubo", renderer->lightsUBO->getBindingPoint());
}

void Renderer::enable(Capability cap) {
	glEnable(static_cast<GLenum>(cap));
}

void Renderer::disable(Capability cap) {
	glDisable(static_cast<GLenum>(cap));
}

void Renderer::blendFunc(BlendFactor sfactor, BlendFactor dfactor) {
	glBlendFunc(static_cast<GLenum>(sfactor), static_cast<GLenum>(dfactor));
}

void Renderer::depthFunc(DepthFunc func) {
	glDepthFunc(static_cast<GLenum>(func));
}

void Renderer::setClearColor(float r, float g, float b, float a) {
	glClearColor(r, g, b, a);
}

void Renderer::clear() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::clearDepth() {
	glClear(GL_DEPTH_BUFFER_BIT);
}

void Renderer::render(const Material *material, const VAO *vao) {
	if(material->getId() != Material::currentUsed) {
		material->getShader()->bind();
	}
	
	material->sendUniforms();
	material->bindTextures();

	vao->draw();
}

void Renderer::render(const Material *material, const Mesh *mesh) {
	render(material, mesh->vao.get());
}

