#include "AudioComponent.h"
#include "BallActor.h"
#include "BoxComponent.h"
#include "FPSActor.h"
#include "FPSCamera.h"
#include "Game.h"
#include "MeshComponent.h"
#include "MoveComponent.h"
#include "PlaneActor.h"
#include "Renderer.h"

FPSActor::FPSActor(Game* game)
    : Actor(game) {
    mMoveComp = new MoveComponent(this);
    mAudioComp = new AudioComponent(this);
    mLastFootstep = 0.0f;
    mFootstep = mAudioComp->PlayEvent("event:/Footstep");
    mFootstep.SetPaused(true);

    mCameraComp = new FPSCamera(this);

    mFPSModel = new Actor(game);
    mFPSModel->SetScale(0.75f);
    mMeshComp = new MeshComponent(mFPSModel);
    mMeshComp->SetMesh(game->GetRenderer()->GetMesh("Assets/Rifle.gpmesh"));

    mBoxComp = new BoxComponent(this);
    const AABB myBox(Vector3(-25.0f, -25.0f, -87.5f), Vector3(25.0f, 25.0f, 87.5f));
    mBoxComp->SetObjectBox(myBox);
    mBoxComp->SetShouldRotate(false);
}

void FPSActor::UpdateActor(const float deltaTime) {
    Actor::UpdateActor(deltaTime);

    FixCollision();

    mLastFootstep -= deltaTime;

    if (!Math::NearZero(mMoveComp->GetForwardSpeed()) && mLastFootstep <= 0.0f) {
        mFootstep.SetPaused(false);
        mFootstep.Restart();
        mLastFootstep = 0.5f;
    }

    const auto modelOffset(Vector3(10.0f, 10.0f, -10.0f));
    auto modelPos = GetPosition();

    modelPos += GetForward() * modelOffset.x;
    modelPos += GetRight() * modelOffset.y;
    modelPos.z += modelOffset.z;

    mFPSModel->SetPosition(modelPos);

    auto q = GetRotation();
    q = Quaternion::Concatenate(q, Quaternion(GetRight(), mCameraComp->GetPitch()));

    mFPSModel->SetRotation(q);
}

void FPSActor::ActorInput(const uint8_t *keyState) {
    auto forwardSpeed = 0.0f;
    auto strafeSpeed = 0.0f;

    if (keyState[SDL_SCANCODE_W]) {
        forwardSpeed += 400.0f;
    }
    if (keyState[SDL_SCANCODE_S]) {
        forwardSpeed -= 400.0f;
    }
    if (keyState[SDL_SCANCODE_A]) {
        strafeSpeed -= 400.0f;
    }
    if (keyState[SDL_SCANCODE_D]) {
        strafeSpeed += 400.0f;
    }

    mMoveComp->SetForwardSpeed(forwardSpeed);
    mMoveComp->SetStrafeSpeed(strafeSpeed);

    int x, y;

    SDL_GetRelativeMouseState(&x, &y);

    constexpr auto maxMouseSpeed = 500;
    constexpr auto maxAngularSpeed = Math::Pi * 8;
    auto angularSpeed = 0.0f;

    if (x != 0) {
        angularSpeed = static_cast<float>(x) / maxMouseSpeed;
        angularSpeed *= maxAngularSpeed;
    }

    mMoveComp->SetAngularSpeed(angularSpeed);

    constexpr auto maxPitchSpeed = Math::Pi * 8;
    auto pitchSpeed = 0.0f;

    if (y != 0) {
        pitchSpeed = static_cast<float>(y) / maxMouseSpeed;
        pitchSpeed *= maxPitchSpeed;
    }

    mCameraComp->SetPitchSpeed(pitchSpeed);
}

void FPSActor::Shoot() {
    Vector3 screenPoint(0.0f, 0.0f, 0.0f);
    const auto start = GetGame()->GetRenderer()->Unproject(screenPoint);
    screenPoint.z = 0.9f;
    const auto end = GetGame()->GetRenderer()->Unproject(screenPoint);
    auto dir = end - start;

    dir.Normalize();

    const auto ball = new BallActor(GetGame());
    ball->SetPlayer(this);
    ball->SetPosition(start + dir * 20.0f);
    ball->RotateToNewForward(dir);
    mAudioComp->PlayEvent("event:/Shot");
}

void FPSActor::SetFootstepSurface(const float value) {
    mFootstep.SetPaused(true);
    mFootstep.SetParameter("Surface", value);
}

void FPSActor::SetVisible(const bool visible) const {
    mMeshComp->SetVisible(visible);
}

void FPSActor::FixCollision() {
    ComputeWorldTransform();

    const auto& playerBox = mBoxComp->GetWorldBox();
    auto pos = GetPosition();
    const auto& planes = GetGame()->GetPlanes();

    for (auto& plane : planes) {
        const auto& planeBox = plane->GetBox()->GetWorldBox();

        if (Intersect(playerBox, planeBox)) {
            const auto dx1 = planeBox.mMax.x - playerBox.mMin.x;
            const auto dx2 = planeBox.mMin.x - playerBox.mMax.x;
            const auto dy1 = planeBox.mMax.y - playerBox.mMin.y;
            const auto dy2 = planeBox.mMin.y - playerBox.mMax.y;
            const auto dz1 = planeBox.mMax.z - playerBox.mMin.z;
            const auto dz2 = planeBox.mMin.z - playerBox.mMax.z;

            const auto dx = Math::Abs(dx1) < Math::Abs(dx2) ? dx1 : dx2;
            const auto dy = Math::Abs(dy1) < Math::Abs(dy2) ? dy1 : dy2;
            const auto dz = Math::Abs(dz1) < Math::Abs(dz2) ? dz1 : dz2;

            if (Math::Abs(dx) <= Math::Abs(dy) && Math::Abs(dx) <= Math::Abs(dz)) {
                pos.x += dx;
            } else if (Math::Abs(dy) <= Math::Abs(dx) && Math::Abs(dy) <= Math::Abs(dz)) {
                pos.y += dy;
            } else {
                pos.z += dz;
            }

            SetPosition(pos);
            mBoxComp->OnUpdateWorldTransform();
        }
    }
}
