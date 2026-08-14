#pragma once

#include "Actor.h"

class BoxComponent;
class Game;

class PlaneActor : public Actor {
public:
    explicit PlaneActor(Game* game);
    ~PlaneActor();

    [[nodiscard]]
    BoxComponent* GetBox() const { return mBox; }

private:
    BoxComponent* mBox;
};
