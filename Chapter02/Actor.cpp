#include "Actor.h"
#include "Component.h"
#include "Game.h"

Actor::Actor(Game* game)
    : mState(EActive)
    , mPosition(Vector2::Zero)
    , mScale(1.0f)
    , mRotation(0.0f)
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
        UpdateComponents(deltaTime);
        UpdateActor(deltaTime);
    }
}

void Actor::UpdateComponents(const float deltaTime) {
    for (const auto& component : mComponents) {
        component->Update(deltaTime);
    }
}

void Actor::UpdateActor(const float deltaTime) {
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
    auto iter = std::ranges::find(mComponents, component);

    if (iter != mComponents.end()) {
        mComponents.erase(iter);
    }
}