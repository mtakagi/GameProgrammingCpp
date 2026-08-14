#pragma once

#include "Component.h"
#include "Collision.h"

class Actor;

class BoxComponent : public Component {
public:
    explicit BoxComponent(Actor* owner, int updateOrder = 100);
    ~BoxComponent() override;

    void OnUpdateWorldTransform() override;

    void SetObjectBox(const AABB& model) { mObjectBox = model; }
    [[nodiscard]]
    const AABB& GetWorldBox() const { return mWorldBox; }

    void SetShouldRotate(bool value) { mShouldRotate = value; }

private:
    AABB mObjectBox;
    AABB mWorldBox;
    bool mShouldRotate;
};
