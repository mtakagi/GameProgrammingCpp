#pragma once

class Actor;
struct InputState;

class Component {
public:
    explicit Component(Actor* owner, int updateOrder = 100);
    virtual ~Component();

    virtual void Update(float deltaTime);
    virtual void ProcessInput(const InputState& state) {}

    virtual void OnUpdateWorldTransform() {}

    [[nodiscard]]
    int GetUpdateOrder() const { return mUpdateOrder; }

protected:
    Actor* mOwner;
    int mUpdateOrder;
};
