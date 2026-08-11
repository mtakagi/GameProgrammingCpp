#pragma once

#include "Component.h"

class Actor;
class Shader;
class Texture;

class SpriteComponent : public Component {
public:
    explicit SpriteComponent(Actor* owner, int drawOrder = 100);
    ~SpriteComponent() override;

    virtual void Draw(Shader* shader);
    virtual void SetTexture(Texture* texture);

    [[nodiscard]]
    int GetDrawOrder() const { return mDrawOrder; }
    [[nodiscard]]
    int GetTexHeight() const { return mTexHeight; }
    [[nodiscard]]
    int GetTexWidth() const { return mTexWidth; }

protected:
    Texture* mTexture;
    int mDrawOrder;
    int mTexWidth;
    int mTexHeight;
};
