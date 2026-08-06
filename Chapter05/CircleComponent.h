#pragma once

#include "Component.h"
#include "Math.h"

class Actor;

class CircleComponent : public Component {
public:
    explicit CircleComponent(Actor* owner);

    void SetRadius(const float radius) { mRadius = radius; }
    [[nodiscard]]
    float GetRadius() const;

    [[nodiscard]]
    const Vector2& GetCenter() const;

private:
    float mRadius;
};

bool Intersect(const CircleComponent& a, const CircleComponent& b);