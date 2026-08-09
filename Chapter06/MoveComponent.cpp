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
        const auto angle = mAngularSpeed * deltaTime;
        const Quaternion inc(Vector3::UnitZ, angle);
        rot = Quaternion::Concatenate(rot, inc);

        mOwner->SetRotation(rot);
    }

    if (!Math::NearZero(mForwardSpeed)) {
        auto pos = mOwner->GetPosition();
        pos += mOwner->GetForward() * mForwardSpeed * deltaTime;

        mOwner->SetPosition(pos);
    }
}