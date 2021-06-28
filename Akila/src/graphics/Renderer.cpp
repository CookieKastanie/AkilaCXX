#include "Akila/graphics/Renderer.hpp"

using namespace Akila;

Renderer::Renderer(std::shared_ptr<Display> &display): display{display} {

}

void Renderer::useDefaultFrameBuffer() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, display->getWidth(), display->getHeight());
}
