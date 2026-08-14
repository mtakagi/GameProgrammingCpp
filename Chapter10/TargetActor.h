#pragma once

#include "Actor.h"

class Game;

class TargetActor : public Actor {
public:
    explicit TargetActor(Game* game);
};
