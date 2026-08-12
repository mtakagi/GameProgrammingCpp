#include "Game.h"
#include "MeshComponent.h"
#include "OrbitActor.h"
#include "OrbitCamera.h"
#include "Renderer.h"

OrbitActor::OrbitActor(Game *game)
    : Actor(game){
    mMeshComp = new MeshComponent(this);
    mMeshComp->SetMesh(game->GetRenderer()->GetMesh("Assets/RacingCar.gpmesh"));

    SetPosition(Vector3(0.0f, 0.0f, -100.0f));

    mCameraComp = new OrbitCamera(this);
}

void OrbitActor::ActorInput(const uint8_t *keyState) {
    int x, y;
    const auto buttons = SDL_GetRelativeMouseState(&x, &y);

    mCameraComp->SetYawSpeed(0.0f);
    mCameraComp->SetPitchSpeed(0.0f);

    if (buttons & SDL_BUTTON(SDL_BUTTON_RIGHT)) {
        constexpr auto maxMouseSpeed = 500;
        constexpr auto maxOrbitSpeed = Math::Pi * 8;
        auto yawSpeed = 0.0f;

        if (x != 0) {
            yawSpeed = static_cast<float>(x) / maxMouseSpeed;
            yawSpeed *= maxOrbitSpeed;
        }

        mCameraComp->SetYawSpeed(-yawSpeed);

        auto pitchSpeed = 0.0f;

        if (y != 0) {
            pitchSpeed = static_cast<float>(y) / maxMouseSpeed;
            pitchSpeed *= maxOrbitSpeed;
        }

        mCameraComp->SetPitchSpeed(pitchSpeed);
    }
}

void OrbitActor::SetVisible(const bool visible) const {
    mMeshComp->SetVisible(visible);
}
