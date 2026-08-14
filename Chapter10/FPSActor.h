#pragma once

#include <cstdint>
#include "Actor.h"
#include "BoxComponent.h"
#include "SoundEvent.h"

class AudioComponent;
class BoxComponent;
class FPSCamera;
class Game;
class MeshComponent;
class MoveComponent;


class FPSActor : public Actor {
public:
    explicit FPSActor(Game* game);

    void UpdateActor(float deltaTime) override;
    void ActorInput(const uint8_t *keyState) override;

    void Shoot();

    void SetFootstepSurface(float value);

    void SetVisible(bool visible) const;

    void FixCollision();

private:
    MoveComponent* mMoveComp;
    AudioComponent* mAudioComp;
    MeshComponent* mMeshComp;
    FPSCamera* mCameraComp;
    BoxComponent* mBoxComp;
    Actor* mFPSModel;
    SoundEvent mFootstep;
    float mLastFootstep;
};
