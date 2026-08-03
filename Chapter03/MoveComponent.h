#pragma once

#include "Component.h"

class MoveComponent : public Component {
public:
    explicit MoveComponent(Actor* owner, int updateOrder = 10);

    void Update(float deltaTime) override;

    [[nodiscard]]
    float GetAngularSpeed() const { return mAngularSpeed; }
    [[nodiscard]]
    float GetForwardSpeed() const { return mForwardSpeed; }
    void SetAngularSpeed(const float speed) { mAngularSpeed = speed; }
    void SetForwardSpeed(const float speed) { mForwardSpeed = speed; }

private:
    float mAngularSpeed;
    float mForwardSpeed;
};
