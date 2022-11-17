#include "akila/renderer/framebuffer.hpp"
#include "akila/window/window.hpp"
#include <iostream>

using namespace akila;

FrameBuffer::FrameBuffer() {
    glGenFramebuffers(1, &id);
    glBindFramebuffer(GL_FRAMEBUFFER, id);
}

FrameBuffer::~FrameBuffer() {
    glDeleteFramebuffers(1, &id);
}

FrameBuffer::FrameBuffer(FrameBuffer &&other) noexcept: id{other.id}, depthTexture{other.depthTexture} {
    for(std::size_t i = 0; i < MAX_ATTACHMENT_COUNT; ++i) {
        textures[i] = other.textures[i];
    }

    other.id = 0;
}

FrameBuffer &FrameBuffer::operator=(FrameBuffer &&other) noexcept {
    id = other.id;
    depthTexture = other.depthTexture;

    for(std::size_t i = 0; i < MAX_ATTACHMENT_COUNT; ++i) {
        textures[i] = other.textures[i];
    }

    other.id = 0;

    return *this;
}

FrameBuffer::Attachement FrameBuffer::nextAttachement(Attachement attachement) {
    if(attachement == Attachement::CUBE_MAP_NEGATIVE_Z) return Attachement::CUBE_MAP_POSITIVE_X;

    GLenum a = static_cast<GLenum>(attachement);
    return static_cast<Attachement>(a + 1);
}

void FrameBuffer::setTexture(Ref<TextureBuffer> const &texture, int unit, Attachement attachment) {
    textures[unit] = texture;
    changeAttachment(unit, attachment);
}

Ref<TextureBuffer> &FrameBuffer::getTexture(int unit) {
    return textures[unit];
}

void FrameBuffer::changeAttachment(int unit, Attachement attachment, unsigned int mip) {
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + unit, static_cast<GLenum>(attachment), textures[unit]->getId(), mip);
}

void FrameBuffer::setDepthTexture(Ref<TextureBuffer> const &texture) {
    depthTexture = texture;
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, texture->getId(), 0);
}

Ref<TextureBuffer> &FrameBuffer::getDepthTexture() {
    return depthTexture;
}

void FrameBuffer::prepare() {
    GLenum buffs[MAX_ATTACHMENT_COUNT];

    int count = 0;
    for(int i = 0; i < MAX_ATTACHMENT_COUNT; ++i) {
        if(textures[i].isValid() && textures[i].raw() != nullptr) {
            buffs[count++] = GL_COLOR_ATTACHMENT0 + i;
        }
    }

    glBindFramebuffer(GL_FRAMEBUFFER, id);
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
    IVec2 texSize = textures[unit]->getSize();
    glBindFramebuffer(GL_FRAMEBUFFER, id);
    glViewport(0, 0, texSize.x, texSize.y);
}

void FrameBuffer::bindWithSize(IVec2 size) {
    glBindFramebuffer(GL_FRAMEBUFFER, id);
    glViewport(0, 0, size.x, size.y);
}

void FrameBuffer::unbind() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::blitToDefault(int unit, TextureBuffer::FilterMode filter) {
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    glBindFramebuffer(GL_READ_FRAMEBUFFER, id);
    glReadBuffer(GL_COLOR_ATTACHMENT0 + unit);

    IVec2 winSize = Window::getSize();
    IVec2 texSize = textures[unit]->getSize();

    glBlitFramebuffer(0, 0,
        texSize.x, texSize.y,
        0, 0,
        winSize.x, winSize.y,
        GL_COLOR_BUFFER_BIT, static_cast<GLenum>(filter)
    );
}

void FrameBuffer::blitTo(int seflUnit, FrameBuffer *fb, TextureBuffer::FilterMode filter) {
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fb->id);
    glBindFramebuffer(GL_READ_FRAMEBUFFER, id);
    glReadBuffer(GL_COLOR_ATTACHMENT0 + seflUnit);

    IVec2 selfTexSize = textures[seflUnit]->getSize();
    IVec2 otherTexSize = fb->textures[0]->getSize();

    glBlitFramebuffer(
        0, 0,
        selfTexSize.x, selfTexSize.y,
        0, 0, otherTexSize.x, otherTexSize.y,
        GL_COLOR_BUFFER_BIT, static_cast<GLenum>(filter)
    );
}

void FrameBuffer::resizeAll(IVec2 size) {
    for(int i = 0; i < MAX_ATTACHMENT_COUNT; ++i) {
        if(textures[i].isValid() && textures[i].raw() != nullptr) {
            textures[i]->setSize(size);
        }
    }

    if(depthTexture.isValid() && depthTexture.raw() != nullptr) {
        depthTexture->setSize(size);
    }
}