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

        if (pos.x < -512.0f) {
            pos.x = 510.0f;
        } else if (pos.x > 512.0f) {
            pos.x = -510.0f;
        }
        if (pos.y < -384.0f) {
            pos.y = 382.0f;
        } else if (pos.y > 384.0f) {
            pos.y = -382.0f;
        }

        mOwner->SetPosition(pos);
    }
}