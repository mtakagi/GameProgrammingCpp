#pragma once

#include <cstdint>
#include "Actor.h"
#include "SoundEvent.h"

class AudioComponent;
class FPSCamera;
class Game;
class MeshComponent;
class MoveComponent;


class FPSActor : public Actor {
public:
    explicit FPSActor(Game* game);

    void UpdateActor(float deltaTime) override;
    void ActorInput(const uint8_t *keyState) override;

    void SetFootstepSurface(float value);

    void SetVisible(bool visible) const;

private:
    MoveComponent* mMoveComp;
    AudioComponent* mAudioComp;
    MeshComponent* mMeshComp;
    FPSCamera* mCameraComp;
    Actor* mFPSModel;
    SoundEvent mFootstep;
    float mLastFootstep;
};
