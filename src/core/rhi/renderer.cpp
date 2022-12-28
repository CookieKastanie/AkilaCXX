#include "akila/core/rhi/renderer.hpp"
#include "akila/core/window/window.hpp"

using namespace akila;

void Renderer::init() {
	enable(Capability::BLEND);
	blendFunc(BlendFactor::SRC_ALPHA, BlendFactor::ONE_MINUS_SRC_ALPHA);
	enable(Capability::DEPTH_TEST);
	enable(Capability::CULL_FACE);
	glFrontFace(GL_CCW);
	cullFace(Face::BACK);
	enable(Capability::TEXTURE_CUBE_MAP_SEAMLESS);
}

void Renderer::bindDefaultFrameBuffer() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	IVec2 size = Window::getSize();
	glViewport(0, 0, size.x, size.y);
}

void Renderer::enable(Capability cap) {
	glEnable(static_cast<GLenum>(cap));
}

void Renderer::disable(Capability cap) {
	glDisable(static_cast<GLenum>(cap));
}

void Renderer::scissor(int x, int y, int width, int height) {
	glScissor(static_cast<GLint>(x), static_cast<GLint>(y),
			  static_cast<GLsizei>(width), static_cast<GLsizei>(height));
}

void Renderer::blendFunc(BlendFactor sfactor, BlendFactor dfactor) {
	glBlendFunc(static_cast<GLenum>(sfactor), static_cast<GLenum>(dfactor));
}

void Renderer::blendEquation(BlendEquation eq) {
	glBlendEquation(static_cast<GLenum>(eq));
}

void Renderer::depthFunc(DepthFunc func) {
	glDepthFunc(static_cast<GLenum>(func));
}

void Renderer::cullFace(Face face) {
	glCullFace(static_cast<GLenum>(face));
}

void Renderer::setClearColor(float r, float g, float b, float a) {
	glClearColor(r, g, b, a);
}

void Renderer::clear() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::clearColor() {
	glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::clearDepth() {
	glClear(GL_DEPTH_BUFFER_BIT);
}

void Renderer::memoryBarrier(Barrier barrier) {
	glMemoryBarrier(static_cast<GLbitfield>(barrier));
}
