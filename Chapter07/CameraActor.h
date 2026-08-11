#pragma once

#include <cstdint>
#include "Actor.h"
#include "SoundEvent.h"

class Game;
class AudioComponent;
class MoveComponent;

class CameraActor : public Actor {
public:
    explicit CameraActor(Game* game);

    void UpdateActor(float deltaTime) override;
    void ActorInput(const uint8_t *keyState) override;

    void SetFootstepSurface(float value);

private:
    MoveComponent* mMoveComp;
    AudioComponent* mAudioComp;
    SoundEvent mFootstep;
    float mLastFootstep;
};
