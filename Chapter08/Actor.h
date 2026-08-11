#pragma once

#include <vector>
#include "Math.h"

class Game;
class Component;
struct InputState;

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

    void ProcessInput(const InputState& state);
    virtual void ActorInput(const InputState& state);

    [[nodiscard]]
    const Vector2& GetPosition() const { return mPosition; }
    void SetPosition(const Vector2& position) { mPosition = position, mRecomputeWorldTransform = true; }
    [[nodiscard]]
    float GetScale() const { return mScale; }
    void SetScale(const float scale) { mScale = scale, mRecomputeWorldTransform = true; }
    [[nodiscard]]
    float GetRotation() const { return mRotation; }
    void SetRotation(const float rotation) { mRotation = rotation, mRecomputeWorldTransform = true; }

    void ComputeWorldTransform();
    [[nodiscard]]
    const Matrix4& GetWorldTransform() const { return mWorldTransform; }

    [[nodiscard]]
    Vector2 GetForward() const { return Vector2(Math::Cos(mRotation), Math::Sin(mRotation)); }

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
    Vector2 mPosition;
    float mScale;
    float mRotation;
    bool mRecomputeWorldTransform;

    std::vector<Component*> mComponents;
    Game* mGame;
};
