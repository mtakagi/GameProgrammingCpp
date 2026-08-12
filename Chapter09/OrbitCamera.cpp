#include "Actor.h"
#include "OrbitCamera.h"

OrbitCamera::OrbitCamera(Actor* owner)
    : CameraComponent(owner)
    , mOffset(-400.0f, 0.0f, 0.0f)
    , mUp(Vector3::UnitZ)
    , mPitchSpeed(0.f)
    , mYawSpeed(0.f) {
}

void OrbitCamera::Update(const float deltaTime) {
    CameraComponent::Update(deltaTime);

    const Quaternion yaw(Vector3::UnitZ, mYawSpeed * deltaTime);

    mOffset = Vector3::Transform(mOffset, yaw);
    mUp = Vector3::Transform(mUp, yaw);

    auto forward = -1.0f * mOffset;
    forward.Normalize();
    auto right = Vector3::Cross(mUp, forward);
    right.Normalize();

    const Quaternion pitch(right, mPitchSpeed * deltaTime);

    mOffset = Vector3::Transform(mOffset, pitch);
    mUp = Vector3::Transform(mUp, pitch);

    const auto target = mOwner->GetPosition();
    const auto cameraPos = target + mOffset;
    const auto view = Matrix4::CreateLookAt(cameraPos, target, mUp);

    SetViewMatrix(view);
}
