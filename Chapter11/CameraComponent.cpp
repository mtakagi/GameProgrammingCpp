#include "Actor.h"
#include "AudioSystem.h"
#include "CameraComponent.h"
#include "Game.h"
#include "Renderer.h"

CameraComponent::CameraComponent(Actor *owner, const int updateOrder)
    : Component(owner, updateOrder) {
}

void CameraComponent::SetViewMatrix(const Matrix4 &view) const {
    const auto game = mOwner->GetGame();
    game->GetRenderer()->SetViewMatrix(view);
    game->GetAudioSystem()->SetListener(view);
}
