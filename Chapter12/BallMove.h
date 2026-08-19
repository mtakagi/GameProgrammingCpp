#pragma once

#include "MoveComponent.h"

class Actor;

class BallMove : public MoveComponent {
public:
    explicit BallMove(Actor* owner);

    void SetPlayer(Actor* player) { mPlayer = player; }
    void Update(float deltaTime) override;

private:
    Actor* mPlayer;
};
