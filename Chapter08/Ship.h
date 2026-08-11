#pragma once

#include "Actor.h"

class Game;
struct InputState;

class Ship : public Actor {
public:
    explicit Ship(Game* game);
    void UpdateActor(float deltaTime) override;
    void ActorInput(const InputState& state) override;

private:
    Vector2 mVelocityDir;
    Vector2 mRotationDir;
    float mSpeed;
    float mLaserCooldown;
};