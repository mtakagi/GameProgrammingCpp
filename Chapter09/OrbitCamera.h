#pragma once

#include "CameraComponent.h"

class Actor;

class OrbitCamera : public CameraComponent {
public:
    explicit OrbitCamera(Actor* owner);

    void Update(float deltaTime) override;

    [[nodiscard]]
    float GetPitchSpeed() const { return mPitchSpeed; }
    [[nodiscard]]
    float GetYawSpeed() const { return mYawSpeed; }

    void SetPitchSpeed(const float speed) { mPitchSpeed = speed; }
    void SetYawSpeed(const float speed) { mYawSpeed = speed; }

private:
    Vector3 mOffset;
    Vector3 mUp;
    float mPitchSpeed;
    float mYawSpeed;
};
