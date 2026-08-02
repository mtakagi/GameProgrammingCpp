#pragma once

#include <SDL.h>
#include "Component.h"

class Actor;

class SpriteComponent : public Component {
public:
    explicit SpriteComponent(Actor* owner, int drawOrder = 100);
    ~SpriteComponent() override;

    virtual void Draw(SDL_Renderer* renderer);
    virtual void SetTexture(SDL_Texture* texture);

    [[nodiscard]]
    int GetDrawOrder() const { return mDrawOrder; }
    [[nodiscard]]
    int GetTexHeight() const { return mTexHeight; }
    [[nodiscard]]
    int GetTexWidth() const { return mTexWidth; }

protected:
    SDL_Texture* mTexture;
    int mDrawOrder;
    int mTexWidth;
    int mTexHeight;
};
