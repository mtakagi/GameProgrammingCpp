#include "AnimSpriteComponent.h"

#include "Actor.h"
#include "Game.h"

AnimSpriteComponent::AnimSpriteComponent(Actor* owner, const int drawOrder)
    : SpriteComponent(owner, drawOrder)
    , mCurrFrame(0.0f)
    , mAnimFPS(24.0f) {
}

void AnimSpriteComponent::Update(const float deltaTime) {
    SpriteComponent::Update(deltaTime);

    if (mAnimTextures.size() > 0) {
        mCurrFrame += mAnimFPS * deltaTime;

        while (mCurrFrame >= mAnimTextures.size()) {
            mCurrFrame -= mAnimTextures.size();
        }

        SetTexture(mAnimTextures[static_cast<int>(mCurrFrame)]);
    }
}

void AnimSpriteComponent::SetAnimTextures(const std::vector<SDL_Texture *> &textures) {
    mAnimTextures = textures;

    if (!mAnimTextures.empty()) {
        mCurrFrame = 0.0f;
        SetTexture(mAnimTextures[0]);
    }
}
