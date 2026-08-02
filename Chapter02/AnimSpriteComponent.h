#pragma once

#include <vector>
#include "SpriteComponent.h"

class Actor;

class AnimSpriteComponent : public SpriteComponent {
public:
    explicit AnimSpriteComponent(Actor* owner, int drawOrder = 100);

    void Update(float deltaTime) override;
    void SetAnimTextures(const std::vector<SDL_Texture*>& textures);
    [[nodiscard]]
    float GetAnimFPS() const { return mAnimFPS; }
    void SetAnimFPS(const float fps) { mAnimFPS = fps; }

private:
    std::vector<SDL_Texture*> mAnimTextures;
    float mCurrFrame;
    float mAnimFPS;
};
