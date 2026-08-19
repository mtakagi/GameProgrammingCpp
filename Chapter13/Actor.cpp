#include <algorithm>
#include "Actor.h"
#include "Component.h"
#include "Game.h"

Actor::Actor(Game* game)
    : mState(EActive)
    , mPosition(Vector3::Zero)
    , mScale(1.0f)
    , mRotation(Quaternion::Identity)
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

void Actor::ProcessInput(const uint8_t *keyState) {
    if (mState == EActive) {
        for (const auto& comp : mComponents) {
            comp->ProcessInput(keyState);
        }

        ActorInput(keyState);
    }
}

void Actor::ActorInput(const uint8_t *keyState) {
}

void Actor::RotateToNewForward(const Vector3 &forward) {
    const auto dot = Vector3::Dot(Vector3::UnitX, forward);
    const auto angle = Math::Acos(dot);

    if (dot > 0.9999f) {
        SetRotation(Quaternion::Identity);
    } else if (dot < -0.9999f) {
        SetRotation(Quaternion(Vector3::UnitZ, Math::Pi));
    } else {
        auto axis = Vector3::Cross(Vector3::UnitX, forward);
        axis.Normalize();
        SetRotation(Quaternion(axis, angle));
    }
}

void Actor::ComputeWorldTransform() {
    if (mRecomputeWorldTransform) {
        mRecomputeWorldTransform = false;
        mWorldTransform = Matrix4::CreateScale(mScale);
        mWorldTransform *= Matrix4::CreateFromQuaternion(mRotation);
        mWorldTransform *= Matrix4::CreateTranslation(mPosition);

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
    auto iter = std::ranges::find(mComponents, component);

    if (iter != mComponents.end()) {
        mComponents.erase(iter);
    }
}