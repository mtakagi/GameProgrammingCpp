#pragma once

class Actor;

class Component {
public:
    explicit Component(Actor* owner, int updateOrder = 100);
    virtual ~Component();

    virtual void Update(float deltaTime);
    [[nodiscard]]
    int GetUpdateOrder() const { return mUpdateOrder; }

protected:
    Actor* mOwner;
    int mUpdateOrder;
};
