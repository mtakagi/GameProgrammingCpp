#pragma once

#include "CameraComponent.h"

class Actor;

class MirrorCamera : public CameraComponent {
public:
    explicit MirrorCamera(Actor* owner);

    void Update(float deltaTime) override;

    void SnapToIdeal();

    void SetHorzDist(const float dist) { mHorzDist = dist; }
    void SetVertDist(const float dist) { mVertDist = dist; }
    void SetTargetDist(const float dist) { mTargetDist = dist; }

private:
    [[nodiscard]]
    Vector3 ComputeCameraPos() const;

    float mHorzDist;
    float mVertDist;
    float mTargetDist;
};
