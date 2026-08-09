#include "CameraActor.h"
#include "Game.h"
#include "MoveComponent.h"
#include "Renderer.h"

CameraActor::CameraActor(Game* game)
    : Actor(game) {
    mMoveComp = new MoveComponent(this);
}

void CameraActor::UpdateActor(const float deltaTime) {
    Actor::UpdateActor(deltaTime);

    const auto cameraPos = GetPosition();
    const auto target = GetPosition() + GetForward();
    const auto up = Vector3::UnitZ;
    const auto view = Matrix4::CreateLookAt(cameraPos, target, up);

    GetGame()->GetRenderer()->SetViewMatrix(view);
}

void CameraActor::ActorInput(const uint8_t *keyState) {
    auto forwardSpeed = 0.0f;
    auto angularSpeed = 0.0f;

    if (keyState[SDL_SCANCODE_W]) {
        forwardSpeed += 300.0f;
    }
    if (keyState[SDL_SCANCODE_S]) {
        forwardSpeed -= 300.0f;
    }
    if (keyState[SDL_SCANCODE_A]) {
        angularSpeed -= Math::TwoPi;
    }
    if (keyState[SDL_SCANCODE_D]) {
        angularSpeed += Math::TwoPi;
    }

    mMoveComp->SetForwardSpeed(forwardSpeed);
    mMoveComp->SetAngularSpeed(angularSpeed);
}
