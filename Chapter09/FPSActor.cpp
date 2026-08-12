#include "AudioComponent.h"
#include "FPSActor.h"
#include "FPSCamera.h"
#include "Game.h"
#include "MeshComponent.h"
#include "MoveComponent.h"
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
}

void FPSActor::UpdateActor(const float deltaTime) {
    Actor::UpdateActor(deltaTime);

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

void FPSActor::SetFootstepSurface(const float value) {
    mFootstep.SetPaused(true);
    mFootstep.SetParameter("Surface", value);
}

void FPSActor::SetVisible(const bool visible) const {
    mMeshComp->SetVisible(visible);
}
