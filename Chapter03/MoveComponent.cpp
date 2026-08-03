#include "Actor.h"
#include "MoveComponent.h"

MoveComponent::MoveComponent(Actor *owner, const int updateOrder)
    : Component(owner, updateOrder)
    , mAngularSpeed(0.f)
    , mForwardSpeed(0.f) {
}

void MoveComponent::Update(const float deltaTime) {
    if (!Math::NearZero(mAngularSpeed)) {
        auto rot = mOwner->GetRotation();
        rot += mAngularSpeed * deltaTime;
        mOwner->SetRotation(rot);
    }

    if (!Math::NearZero(mForwardSpeed)) {
        auto pos = mOwner->GetPosition();
        pos += mOwner->GetForward() * mForwardSpeed * deltaTime;

        if (pos.x < 0.0f) {
            pos.x = 1022.0f;
        } else if (pos.x > 1024.0f) {
            pos.x = 2.0f;
        }

        if (pos.y < 0.0f) {
            pos.y = 766.0f;
        } else if (pos.y > 766.0f) {
            pos.y = 2.0f;
        }

        mOwner->SetPosition(pos);
    }
}