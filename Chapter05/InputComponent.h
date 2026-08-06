#pragma once

#include <cstdint>
#include "MoveComponent.h"

class Actor;

class InputComponent :public MoveComponent {
public:
    explicit InputComponent(Actor* owner);

    void ProcessInput(const uint8_t *keyState) override;

    [[nodiscard]]
    float GetMaxForwardSpeed() const { return mMaxForwardSpeed; }
    void SetMaxForwardSpeed(const float maxForwardSpeed) { mMaxForwardSpeed = maxForwardSpeed; }
    [[nodiscard]]
    float GetMaxAngularSpeed() const { return mMaxAngularSpeed; }
    void SetMaxAngularSpeed(const float maxAngularSpeed) { mMaxAngularSpeed = maxAngularSpeed; }
    [[nodiscard]]
    int GetForwardKey() const { return mForwardKey; }
    void SetForwardKey(const int forwardKey) { mForwardKey = forwardKey; }
    [[nodiscard]]
    int GetBackKey() const { return mBackKey; }
    void SetBackKey(const int backKey) { mBackKey = backKey; }
    [[nodiscard]]
    int GetClockwiseKey() const { return mClockwiseKey; }
    void SetClockwiseKey(const int clockwiseKey) { mClockwiseKey = clockwiseKey; }
    [[nodiscard]]
    int GetCounterClockwiseKey() const { return mCounterClockwiseKey; }
    void SetCounterClockwiseKey(const int counterClockwiseKey) { mCounterClockwiseKey = counterClockwiseKey; }

private:
    float mMaxForwardSpeed;
    float mMaxAngularSpeed;
    int mForwardKey;
    int mBackKey;
    int mClockwiseKey;
    int mCounterClockwiseKey;
};
