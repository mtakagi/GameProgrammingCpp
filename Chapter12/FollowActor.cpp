#include "Animation.h"
#include "FollowActor.h"
#include "FollowCamera.h"
#include "Game.h"
#include "MoveComponent.h"
#include "Renderer.h"
#include "Skeleton.h"
#include "SkeletalMeshComponent.h"

FollowActor::FollowActor(Game* game)
    : Actor(game)
    , mMoving(false) {
    mMeshComp = new SkeletalMeshComponent(this);
    mMeshComp->SetMesh(game->GetRenderer()->GetMesh("Assets/CatWarrior.gpmesh"));
    mMeshComp->SetSkeleton(game->GetSkeleton("Assets/CatWarrior.gpskel"));
    mMeshComp->PlayAnimation(game->GetAnimation("Assets/CatActionIdle.gpanim"));

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

    if (!mMoving && !Math::NearZero(forwardSpeed)) {
        mMoving = true;
        mMeshComp->PlayAnimation(GetGame()->GetAnimation("Assets/CatRunSprint.gpanim"));
    } else if (mMoving && Math::NearZero(forwardSpeed)) {
        mMoving = false;
        mMeshComp->PlayAnimation(GetGame()->GetAnimation("Assets/CatActionIdle.gpanim"));
    }

    mMoveComp->SetForwardSpeed(forwardSpeed);
    mMoveComp->SetAngularSpeed(angularSpeed);
}

void FollowActor::SetVisible(const bool visible) const {
    mMeshComp->SetVisible(visible);
}
