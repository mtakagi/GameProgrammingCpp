#pragma once

#include "CameraComponent.h"

class Actor;

class FollowCamera : public CameraComponent {
public:
    explicit FollowCamera(Actor* owner);

    void Update(float deltaTime) override;

    void SnapToIdeal();

    void SetHorzDist(const float dist) { mHorzDist = dist; }
    void SetVertDist(const float dist) { mVertDist = dist; }
    void SetTargetDist(const float dist) { mTargetDist = dist; }
    void SetSpringConstant(const float spring) { mSpringConstant = spring; }

private:
    [[nodiscard]]
    Vector3 ComputeCameraPos() const;

    Vector3 mActualPos;
    Vector3 mVelocity;
    float mHorzDist;
    float mVertDist;
    float mTargetDist;
    float mSpringConstant;
};
