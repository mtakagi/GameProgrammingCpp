#pragma once

#include "Actor.h"

class Game;
class MoveComponent;

class CameraActor : public Actor {
public:
    explicit CameraActor(Game* game);

    void UpdateActor(float deltaTime) override;
    void ActorInput(const uint8_t *keyState) override;

private:
    MoveComponent* mMoveComp;
};
