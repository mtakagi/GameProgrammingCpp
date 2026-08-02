#pragma once

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

    [[nodiscard]]
    const Vector2& GetPosition() const { return mPosition; }
    void SetPosition(const Vector2& position) { mPosition = position; }
    [[nodiscard]]
    float GetScale() const { return mScale; }
    void SetScale(const float scale) { mScale = scale; }
    [[nodiscard]]
    float GetRotation() const { return mRotation; }
    void SetRotation(const float rotation) { mRotation = rotation; }

    void AddComponent(Component* component);
    void RemoveComponent(Component* component);

    [[nodiscard]]
    State GetState() const { return mState; }
    void SetState(const State state) { mState = state; }

    [[nodiscard]]
    Game* GetGame() const { return mGame; }

private:
    State mState;
    Vector2 mPosition;
    float mScale;
    float mRotation;
    std::vector<Component*> mComponents;
    Game* mGame;
};
