#include "Akila/graphics/gl/FrameBuffer.hpp"
//#include "DemoTER/glwrapper/Display.hpp"

#include <iostream>

using namespace Akila;

FrameBuffer::FrameBuffer() {
    glGenFramebuffers(1, &id);
    glBindFramebuffer(GL_FRAMEBUFFER, id);
}

FrameBuffer::~FrameBuffer() {
    glDeleteFramebuffers(1, &id);
}

void FrameBuffer::setTexture(const std::shared_ptr<Texture> &texture, int unit) {
    textures[unit] = texture;
    changeAttachment(unit, GL_TEXTURE_2D);
}

void FrameBuffer::setTexture(const std::shared_ptr<CubeMapTexture> &texture, int unit) {
    textures[unit] = texture;
    changeAttachment(unit, GL_TEXTURE_CUBE_MAP_POSITIVE_X);
}

std::shared_ptr<TextureBuffer> FrameBuffer::getTexture(int unit) {
    return textures[unit];
}

void FrameBuffer::changeAttachment(int unit, GLenum attachment, unsigned int mip) {
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + unit, attachment, textures[unit]->getId(), mip);
}

void FrameBuffer::setDepthTexture(const std::shared_ptr<DepthTexture> &texture) {
    depthTexture = texture;
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, texture->getId(), 0);
}

std::shared_ptr<TextureBuffer> FrameBuffer::getDepthTexture() {
    return depthTexture;
}

void FrameBuffer::prepare() {
    GLenum buffs[MAX_ATTACHMENT_COUNT];

    int count = 0;
    for(int i = 0; i < MAX_ATTACHMENT_COUNT; ++i) {
        if(textures[i].get() != nullptr) {
            buffs[count++] = GL_COLOR_ATTACHMENT0 + i;
        }
    }

    glDrawBuffers(count, buffs);

    int status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if(status != GL_FRAMEBUFFER_COMPLETE) {
        std::cout << "Framebuffer incomplete. Status: ";
        switch(status) {
            case GL_FRAMEBUFFER_UNDEFINED: std::cout << "GL_FRAMEBUFFER_UNDEFINED"; break;
            case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT: std::cout << "GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT"; break;
            case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT: std::cout << "GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT"; break;
            case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER: std::cout << "GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER"; break;
            case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER: std::cout << "GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER"; break;
            case GL_FRAMEBUFFER_UNSUPPORTED: std::cout << "GL_FRAMEBUFFER_UNSUPPORTED"; break;
            case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE: std::cout << "GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE"; break;
            case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS: std::cout << "GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS"; break;
            default: std::cout << "UNKNWON"; break;
        }

        std::cout << std::endl;
    }
}

void FrameBuffer::bind(int unit) {
    glBindFramebuffer(GL_FRAMEBUFFER, id);
    TextureBuffer *t = textures[unit].get();
    glViewport(0, 0, t->getWidth(), t->getHeight());
}

void FrameBuffer::bindWithSize(int width, int height) {
    glBindFramebuffer(GL_FRAMEBUFFER, id);
    glViewport(0, 0, width, height);
}

void FrameBuffer::unbind() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::blitToScreen(int unit, TextureBuffer::FilterMode filter) {
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    glBindFramebuffer(GL_READ_FRAMEBUFFER, id);
    glReadBuffer(GL_COLOR_ATTACHMENT0 + unit);
    //Display *d = Display::getInstance();
    //TexBuffer *t = textures[unit].get();
    //glBlitFramebuffer(0, 0, t->getWidth(), t->getHeight(), 0, 0, d->getWidth(), d->getHeight(), GL_COLOR_BUFFER_BIT, filter);

    TextureBuffer *t = textures[unit].get();
    glBlitFramebuffer(0, 0, t->getWidth(), t->getHeight(), 0, 0, t->getWidth(), t->getHeight(), GL_COLOR_BUFFER_BIT, filter);
}

void FrameBuffer::blitTo(int seflUnit, FrameBuffer *fb, TextureBuffer::FilterMode filter) {
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fb->id);
    glBindFramebuffer(GL_READ_FRAMEBUFFER, id);
    glReadBuffer(GL_COLOR_ATTACHMENT0 + seflUnit);

    TextureBuffer *selfTex = textures[seflUnit].get();
    TextureBuffer *otherTex = fb->textures[0].get();
    glBlitFramebuffer(0, 0, selfTex->getWidth(), selfTex->getHeight(), 0, 0, otherTex->getWidth(), otherTex->getHeight(), GL_COLOR_BUFFER_BIT, filter);
}

void FrameBuffer::resizeAll(int width, int height) {
    for(int i = 0; i < MAX_ATTACHMENT_COUNT; ++i) {
        if(textures[i].get() != nullptr) {
            textures[i]->setSize(width, height);
        }
    }

    if(depthTexture.get() != nullptr) {
        depthTexture->setSize(width, height);
    }
}
