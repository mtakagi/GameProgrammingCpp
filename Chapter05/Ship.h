#pragma once

#include "Actor.h"

class Game;

class Ship : public Actor {
public:
    explicit Ship(Game* game);
    void UpdateActor(float deltaTime) override;
    void ActorInput(const uint8_t *keyState) override;

private:
    float mLaserCooldown;
};