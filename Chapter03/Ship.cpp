#include "Game.h"
#include "Ship.h"
#include "SpriteComponent.h"
#include "InputComponent.h"
#include "Laser.h"

Ship::Ship(Game* game)
    : Actor(game)
    , mLaserCooldown(0.0f) {
    const auto sc = new SpriteComponent(this);
    sc->SetTexture(game->GetTexture("Assets/ship.png"));

    const auto ic = new InputComponent(this);
    ic->SetForwardKey(SDL_SCANCODE_W);
    ic->SetBackKey(SDL_SCANCODE_S);
    ic->SetClockwiseKey(SDL_SCANCODE_A);
    ic->SetCounterClockwiseKey(SDL_SCANCODE_D);
    ic->SetMaxForwardSpeed(300.0f);
    ic->SetMaxAngularSpeed(Math::TwoPi);
}

void Ship::UpdateActor(const float deltaTime) {
    mLaserCooldown -= deltaTime;
}

void Ship::ActorInput(const uint8_t *keyState) {
    if (keyState[SDL_SCANCODE_SPACE] && mLaserCooldown <= 0.0f) {
        const auto laser = new Laser(GetGame());
        laser->SetPosition(GetPosition());
        laser->SetRotation(GetRotation());

        mLaserCooldown = 0.5f;
    }
}
