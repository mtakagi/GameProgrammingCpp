#include "Actor.h"
#include "FPSCamera.h"

FPSCamera::FPSCamera(Actor* owner)
    : CameraComponent(owner)
    , mPitchSpeed(0.0f)
    , mMaxPitch(Math::Pi / 3.0f)
    , mPitch(0.0f) {
}

void FPSCamera::Update(const float deltaTime) {
    CameraComponent::Update(deltaTime);

    const auto cameraPos = mOwner->GetPosition();

    mPitch += mPitchSpeed * deltaTime;
    mPitch = Math::Clamp(mPitch, -mMaxPitch, mMaxPitch);

    const Quaternion q(mOwner->GetRight(), mPitch);
    const auto viewForward = Vector3::Transform(mOwner->GetForward(), q);
    const auto target = cameraPos + viewForward * 100.0f;
    const auto up = Vector3::Transform(Vector3::UnitZ, q);
    const auto view = Matrix4::CreateLookAt(cameraPos, target, up);

    SetViewMatrix(view);
}