#pragma once

#include <cstdint>
#include "Actor.h"

class Game;
class SplineCamera;

class SplineActor : public Actor {
public:
    explicit SplineActor(Game* game);

    void ActorInput(const uint8_t *keyState) override;

    void RestartSpline() const;

private:
    SplineCamera* mCameraComp;
};
