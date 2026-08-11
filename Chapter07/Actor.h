#pragma once

#include <cstdint>
#include <vector>
#include "Math.h"

class Game;
class Component;

class Actor {
public:
    enum State {
        EActive,
        EPaused,
        EDead
    };

    explicit Actor(Game* game);
    virtual ~Actor();

    virtual void Update(float deltaTime);
    virtual void UpdateComponents(float deltaTime);
    virtual void UpdateActor(float deltaTime);

    void ProcessInput(const uint8_t* keyState);
    virtual void ActorInput(const uint8_t* keyState);

    [[nodiscard]]
    const Vector3& GetPosition() const { return mPosition; }
    void SetPosition(const Vector3& position) { mPosition = position, mRecomputeWorldTransform = true; }
    [[nodiscard]]
    float GetScale() const { return mScale; }
    void SetScale(const float scale) { mScale = scale, mRecomputeWorldTransform = true; }
    [[nodiscard]]
    const Quaternion& GetRotation() const { return mRotation; }
    void SetRotation(const Quaternion& rotation) { mRotation = rotation, mRecomputeWorldTransform = true; }

    void ComputeWorldTransform();
    [[nodiscard]]
    const Matrix4& GetWorldTransform() const { return mWorldTransform; }

    [[nodiscard]]
    Vector3 GetForward() const { return Vector3::Transform(Vector3::UnitX, mRotation); }

    void AddComponent(Component* component);
    void RemoveComponent(Component* component);

    [[nodiscard]]
    State GetState() const { return mState; }
    void SetState(const State state) { mState = state; }

    [[nodiscard]]
    Game* GetGame() const { return mGame; }

private:
    State mState;
    Matrix4 mWorldTransform;
    Vector3 mPosition;
    float mScale;
    Quaternion mRotation;
    bool mRecomputeWorldTransform;

    std::vector<Component*> mComponents;
    Game* mGame;
};
