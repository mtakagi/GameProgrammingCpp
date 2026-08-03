#include "CircleComponent.h"

#include "Actor.h"

CircleComponent::CircleComponent(Actor* owner)
    : Component(owner)
    , mRadius(0.0f) {
}

const Vector2& CircleComponent::GetCenter() const {
    return mOwner->GetPosition();
}

float CircleComponent::GetRadius() const {
    return mOwner->GetScale() *  mRadius;
}

bool Intersect(const CircleComponent& a, const CircleComponent& b) {
    const auto diff = a.GetCenter() - b.GetCenter();
    const auto distSq = diff.LengthSq();
    auto radiiSq = a.GetRadius() + b.GetRadius();

    radiiSq *= radiiSq;

    return distSq < radiiSq;
}