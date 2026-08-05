#pragma once

#include "Actor.h"

class CircleComponent;
class Game;

class Enemy : public Actor {
public:
    explicit Enemy(Game* game);
    ~Enemy() override;

    void UpdateActor(float deltaTime) override;
    [[nodiscard]]
    CircleComponent* GetCircle() const { return mCircle; }

private:
    CircleComponent* mCircle;
};
