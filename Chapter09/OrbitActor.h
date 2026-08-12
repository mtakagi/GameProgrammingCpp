#pragma once

#include <cstdint>
#include "Actor.h"

class Game;
class OrbitCamera;
class MeshComponent;

class OrbitActor : public Actor {
public:
    explicit OrbitActor(Game* game);

    void ActorInput(const uint8_t *keyState) override;

    void SetVisible(bool visible) const;

private:
    OrbitCamera* mCameraComp;
    MeshComponent* mMeshComp;
};
