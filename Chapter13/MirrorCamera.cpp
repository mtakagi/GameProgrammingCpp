#include "Actor.h"
#include "Game.h"
#include "Renderer.h"
#include "MirrorCamera.h"

MirrorCamera::MirrorCamera(Actor* owner)
    : CameraComponent(owner)
    , mHorzDist(150.0f)
    , mVertDist(200.0f)
    , mTargetDist(400.0f) {
}

void MirrorCamera::Update(const float deltaTime) {
    CameraComponent::Update(deltaTime);

    const auto idealPos = ComputeCameraPos();
    const auto target = mOwner->GetPosition() - mOwner->GetForward() * mTargetDist;
    const auto view = Matrix4::CreateLookAt(idealPos, target, Vector3::UnitZ);

    mOwner->GetGame()->GetRenderer()->SetMirrorView(view);
}

void MirrorCamera::SnapToIdeal() {
    const auto idealPos = ComputeCameraPos();
    const auto target = mOwner->GetPosition() - mOwner->GetForward() * mTargetDist;
    const auto view = Matrix4::CreateLookAt(idealPos, target, Vector3::UnitZ);

    mOwner->GetGame()->GetRenderer()->SetMirrorView(view);
}

Vector3 MirrorCamera::ComputeCameraPos() const {
    auto cameraPos = mOwner->GetPosition();
    cameraPos += mOwner->GetForward() * mHorzDist;
    cameraPos += Vector3::UnitZ * mVertDist;

    return cameraPos;
}