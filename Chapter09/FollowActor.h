#pragma once

#include <cstdint>
#include "Actor.h"

class FollowCamera;
class Game;
class MeshComponent;
class MoveComponent;

class FollowActor : public Actor {
public:
    explicit FollowActor(Game* game);

    void ActorInput(const uint8_t *keyState) override;

    void SetVisible(bool visible) const;

private:
    MoveComponent* mMoveComp;
    FollowCamera* mCameraComp;
    MeshComponent* mMeshComp;
};
