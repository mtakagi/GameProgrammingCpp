#pragma once

#include <cstdint>

class Actor;

class Component {
public:
    explicit Component(Actor* owner, int updateOrder = 100);
    virtual ~Component();

    virtual void Update(float deltaTime);
    virtual void ProcessInput(const uint8_t* keyState) {}

    virtual void OnUpdateWorldTransform() {}

    [[nodiscard]]
    Actor* GetOwner() const { return mOwner; }
    [[nodiscard]]
    int GetUpdateOrder() const { return mUpdateOrder; }

protected:
    Actor* mOwner;
    int mUpdateOrder;
};
