#include <GL/glew.h>
#include "GBuffer.h"
#include "Texture.h"

GBuffer::GBuffer()
    : mBufferID(0) {
}

GBuffer::~GBuffer() = default;

bool GBuffer::Create(const int width, const int height) {
    glGenFramebuffers(1, &mBufferID);
    glBindFramebuffer(GL_FRAMEBUFFER, mBufferID);

    GLuint depthBuffer;
    glGenRenderbuffers(1, &depthBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);

    for (auto i = 0; i < NUM_GBUFFER_TEXTURES; i++) {
        const auto tex = new Texture();
        tex->CreateForRendering(width, height, GL_RGB32F);
        mTextures.emplace_back(tex);
        glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, tex->GetTextureID(), 0);
    }

    std::vector<GLenum> attachments;

    for (auto i = 0; i < NUM_GBUFFER_TEXTURES; i++) {
        attachments.emplace_back(GL_COLOR_ATTACHMENT0 + i);
    }

    glDrawBuffers(static_cast<GLsizei>(attachments.size()), attachments.data());

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        Destroy();

        return false;
    }

    return true;
}

void GBuffer::Destroy() {
    glDeleteFramebuffers(1, &mBufferID);

    for (const auto& t : mTextures) {
        t->Unload();
        delete t;
    }
}

Texture* GBuffer::GetTexture(const Type type) {
    if (!mTextures.empty()) {
        return mTextures[type];
    }

    return nullptr;
}

void GBuffer::SetTexturesActive() {
    for (auto i = 0; i < NUM_GBUFFER_TEXTURES; i++) {
        mTextures[i]->SetActive(i);
    }
}
