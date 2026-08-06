#pragma once
#include "Actor.h"

class CircleComponent;

class Asteroid : public Actor {
public:
    explicit Asteroid(Game* game);
    ~Asteroid() override;

    [[nodiscard]]
    CircleComponent* GetCircle() const { return mCircle; }

private:
    CircleComponent* mCircle;
};
