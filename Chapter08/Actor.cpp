#include <algorithm>
#include "Actor.h"
#include "Component.h"
#include "Game.h"

Actor::Actor(Game* game)
    : mState(EActive)
    , mPosition(Vector2::Zero)
    , mScale(1.0f)
    , mRotation(0.0f)
    , mRecomputeWorldTransform(true)
    , mGame(game)
{
    mGame->AddActor(this);
}

Actor::~Actor() {
    mGame->RemoveActor(this);

    while (!mComponents.empty()) {
        delete mComponents.back();
    }
}

void Actor::Update(const float deltaTime) {
    if (mState == EActive) {
        ComputeWorldTransform();

        UpdateComponents(deltaTime);
        UpdateActor(deltaTime);

        ComputeWorldTransform();
    }
}

void Actor::UpdateComponents(const float deltaTime) {
    for (const auto& component : mComponents) {
        component->Update(deltaTime);
    }
}

void Actor::UpdateActor(const float deltaTime) {
}

void Actor::ProcessInput(const InputState& state) {
    if (mState == EActive) {
        for (const auto& comp : mComponents) {
            comp->ProcessInput(state);
        }

        ActorInput(state);
    }
}

void Actor::ActorInput(const InputState& state) {
}

void Actor::ComputeWorldTransform() {
    if (mRecomputeWorldTransform) {
        mRecomputeWorldTransform = false;
        mWorldTransform = Matrix4::CreateScale(mScale);
        mWorldTransform *= Matrix4::CreateRotationZ(mRotation);
        mWorldTransform *= Matrix4::CreateTranslation(Vector3(mPosition.x, mPosition.y, 0));

        for (const auto& component : mComponents) {
            component->OnUpdateWorldTransform();
        }
    }
}

void Actor::AddComponent(Component* component) {
    const int myOrder = component->GetUpdateOrder();
    auto iter = mComponents.begin();

    for (; iter != mComponents.end(); ++iter) {
        if (myOrder < (*iter)->GetUpdateOrder()) {
            break;
        }
    }

    mComponents.insert(iter, component);
}

void Actor::RemoveComponent(Component* component) {
    const auto iter = std::ranges::find(mComponents, component);

    if (iter != mComponents.end()) {
        mComponents.erase(iter);
    }
}