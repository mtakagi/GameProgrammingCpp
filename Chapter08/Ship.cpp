#include "Game.h"
#include "Ship.h"
#include "SpriteComponent.h"
#include "InputSystem.h"
#include "Laser.h"

Ship::Ship(Game* game)
    : Actor(game)
    , mVelocityDir()
    , mRotationDir()
    , mSpeed(400.0f)
    , mLaserCooldown(0.0f) {
    const auto sc = new SpriteComponent(this);
    sc->SetTexture(game->GetTexture("Assets/Ship.png"));
}

void Ship::UpdateActor(const float deltaTime) {
    mLaserCooldown -= deltaTime;

    auto pos = GetPosition();
    pos += mVelocityDir * mSpeed * deltaTime;
    SetPosition(pos);

    const auto angle = Math::Atan2(mRotationDir.y, mRotationDir.x);
    SetRotation(angle);
}

void Ship::ActorInput(const InputState& state) {
    if (state.Controller.GetRightTrigger() > 0.25f && mLaserCooldown <= 0.0f) {
        const auto laser = new Laser(GetGame());
        laser->SetPosition(GetPosition());
        laser->SetRotation(GetRotation());

        mLaserCooldown = 0.25f;
    }

    if (state.Controller.GetIsConnected()) {
        mVelocityDir = state.Controller.GetLeftStick();

        if (!Math::NearZero(state.Controller.GetRightStick().Length())) {
            mRotationDir = state.Controller.GetRightStick();
        }
    }
}
