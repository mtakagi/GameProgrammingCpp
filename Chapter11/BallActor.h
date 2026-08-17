#pragma once

#include "Actor.h"

class AudioComponent;
class BallMove;
class Game;

class BallActor : public Actor {
public:
    explicit BallActor(Game *game);

    void UpdateActor(float deltaTime) override;
    void SetPlayer(Actor *player) const;

    void HitTarget() const;

private:
    AudioComponent* mAudioComp;
    BallMove* mMyMove;
    float mLifeSpan;
};
