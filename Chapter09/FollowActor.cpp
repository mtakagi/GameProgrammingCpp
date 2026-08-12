#include "FollowActor.h"
#include "FollowCamera.h"
#include "Game.h"
#include "MeshComponent.h"
#include "MoveComponent.h"
#include "Renderer.h"

FollowActor::FollowActor(Game* game)
    : Actor(game) {
    mMeshComp = new MeshComponent(this);
    mMeshComp->SetMesh(game->GetRenderer()->GetMesh("Assets/RacingCar.gpmesh"));

    SetPosition(Vector3(0.0f, 0.0f, -100.0f));

    mMoveComp = new MoveComponent(this);
    mCameraComp = new FollowCamera(this);
    mCameraComp->SnapToIdeal();
}

void FollowActor::ActorInput(const uint8_t *keyState) {
    auto forwardSpeed = 0.0f;
    auto angularSpeed = 0.0f;

    if (keyState[SDL_SCANCODE_W]) {
        forwardSpeed += 400.0f;
    }
    if (keyState[SDL_SCANCODE_S]) {
        forwardSpeed -= 400.0f;
    }
    if (keyState[SDL_SCANCODE_A]) {
        angularSpeed -= Math::Pi;
    }
    if (keyState[SDL_SCANCODE_D]) {
        angularSpeed += Math::Pi;
    }

    mMoveComp->SetForwardSpeed(forwardSpeed);
    mMoveComp->SetAngularSpeed(angularSpeed);

    if (!Math::NearZero(forwardSpeed)) {
        mCameraComp->SetHorzDist(500.0f);
    } else {
        mCameraComp->SetHorzDist(350.0f);
    }
}

void FollowActor::SetVisible(const bool visible) const {
    mMeshComp->SetVisible(visible);
}
