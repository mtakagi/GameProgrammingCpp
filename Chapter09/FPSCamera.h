#pragma once

#include "CameraComponent.h"

class Actor;

class FPSCamera : public CameraComponent {
public:
    explicit FPSCamera(Actor* owner);

    void Update(float deltaSeconds) override;

    [[nodiscard]]
    float GetPitch() const { return mPitch; }
    [[nodiscard]]
    float GetPitchSpeed() const { return mPitchSpeed; }
    [[nodiscard]]
    float GetMaxPitch() const { return mMaxPitch; }

    void SetPitchSpeed(const float speed) { mPitchSpeed = speed; }
    void SetMaxPitch(const float pitch) { mMaxPitch = pitch; }

private:
    float mPitchSpeed;
    float mMaxPitch;
    float mPitch;
};
