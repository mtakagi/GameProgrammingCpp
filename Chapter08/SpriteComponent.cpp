#include "Actor.h"
#include "Game.h"
#include "Shader.h"
#include "SpriteComponent.h"
#include "Texture.h"

SpriteComponent::SpriteComponent(Actor *owner, const int drawOrder)
    : Component(owner)
    , mTexture(nullptr)
    , mDrawOrder(drawOrder)
    , mTexWidth(0)
    , mTexHeight(0) {
    mOwner->GetGame()->AddSprite(this);
}

SpriteComponent::~SpriteComponent() {
    mOwner->GetGame()->RemoveSprite(this);
}

void SpriteComponent::Draw(Shader* shader) {
    if (mTexture) {
        const auto scaleMat = Matrix4::CreateScale(static_cast<float>(mTexWidth), static_cast<float>(mTexHeight), 1.0f);
        const auto world = scaleMat * mOwner->GetWorldTransform();

        shader->SetMatrixUniform("uWorldTransform", world);
        mTexture->SetActive();
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
    }
}

void SpriteComponent::SetTexture(Texture* texture) {
    mTexture = texture;
    mTexWidth = texture->GetWidth();
    mTexHeight = texture->GetHeight();
}
