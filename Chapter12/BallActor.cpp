#include "AudioComponent.h"
#include "BallActor.h"
#include "BallMove.h"
#include "Game.h"
#include "Mesh.h"
#include "MeshComponent.h"
#include "Renderer.h"

BallActor::BallActor(Game *game)
    : Actor(game)
    , mLifeSpan(2.0f) {
    const auto mc = new MeshComponent(this);
    const auto mesh = GetGame()->GetRenderer()->GetMesh("Assets/Sphere.gpmesh");
    mc->SetMesh(mesh);
    mMyMove = new BallMove(this);
    mMyMove->SetForwardSpeed(1500.0f);
    mAudioComp = new AudioComponent(this);
}

void BallActor::UpdateActor(const float deltaTime) {
    Actor::UpdateActor(deltaTime);

    mLifeSpan -= deltaTime;

    if (mLifeSpan <= 0.0f) {
        SetState(EDead);
    }
}

void BallActor::SetPlayer(Actor *player) const {
    mMyMove->SetPlayer(player);
}

void BallActor::HitTarget() const {
    mAudioComp->PlayEvent("event:/Ding");
}
