#include "Actor.h"
#include "FollowCamera.h"

FollowCamera::FollowCamera(Actor* owner)
    : CameraComponent(owner)
    , mHorzDist(350.0f)
    , mVertDist(150.0f)
    , mTargetDist(100.0f)
    , mSpringConstant(64.0f) {
}

void FollowCamera::Update(const float deltaTime) {
    CameraComponent::Update(deltaTime);

    const auto dampening = 2.0f * Math::Sqrt(mSpringConstant);
    const auto idealPos = ComputeCameraPos();
    const auto diff = mActualPos - idealPos;
    const auto acel = -mSpringConstant * diff - dampening * mVelocity;

    mVelocity += acel * deltaTime;
    mActualPos += mVelocity * deltaTime;

    const auto target = mOwner->GetPosition() + mOwner->GetForward() * mTargetDist;
    const auto view = Matrix4::CreateLookAt(mActualPos, target, Vector3::UnitZ);

    SetViewMatrix(view);
}

void FollowCamera::SnapToIdeal() {
    mActualPos = ComputeCameraPos();
    mVelocity = Vector3::Zero;

    const auto target = mOwner->GetPosition() + mOwner->GetForward() * mTargetDist;
    const auto view = Matrix4::CreateLookAt(mActualPos, target, Vector3::UnitZ);

    SetViewMatrix(view);
}

Vector3 FollowCamera::ComputeCameraPos() const {
    auto cameraPos = mOwner->GetPosition();
    cameraPos -= mOwner->GetForward() * mHorzDist;
    cameraPos += Vector3::UnitZ * mVertDist;

    return cameraPos;
}