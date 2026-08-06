#include "InputComponent.h"

InputComponent::InputComponent(Actor* owner)
    : MoveComponent(owner)
    , mMaxForwardSpeed(0.0f)
    , mMaxAngularSpeed(0.0f)
    , mForwardKey(0.0f)
    , mBackKey(0.0f)
    , mClockwiseKey(0.0f)
    , mCounterClockwiseKey(0.0f) {
}

void InputComponent::ProcessInput(const uint8_t *keyState) {
    auto forwardSpeed = 0.0f;

    if (keyState[mForwardKey]) {
        forwardSpeed += mMaxForwardSpeed;
    }

    if (keyState[mBackKey]) {
        forwardSpeed -= mMaxForwardSpeed;
    }

    SetForwardSpeed(forwardSpeed);

    auto angularSpeed = 0.0f;

    if (keyState[mClockwiseKey]) {
        angularSpeed += mMaxAngularSpeed;
    }
    if (keyState[mCounterClockwiseKey]) {
        angularSpeed -= mMaxAngularSpeed;
    }

    SetAngularSpeed(angularSpeed);
}
