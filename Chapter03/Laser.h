#pragma once

#include "Actor.h"

class CircleComponent;

class Laser : public Actor {
public:
    explicit Laser(Game* game);

    void UpdateActor(float deltaTime) override;

private:
    CircleComponent* mCircle;
    float mDeathTimer;
};
