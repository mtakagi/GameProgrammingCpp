#pragma once

#include "Actor.h"

class Game;

class Ship : public Actor {
public:
    explicit Ship(Game* game);
    void UpdateActor(float deltaTime) override;
    void ProcessKeyboard(const uint8_t* state);
    [[nodiscard]]
    float GetRightSpeed() const { return mRightSpeed; }
    [[nodiscard]]
    float GetDownSpeed() const { return mDownSpeed; }

private:
    float mRightSpeed;
    float mDownSpeed;
};