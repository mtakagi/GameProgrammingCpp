#include "Actor.h"
#include "Component.h"

Component::Component(Actor* owner, const int updateOrder)
    : mOwner(owner)
    , mUpdateOrder(updateOrder) {
    mOwner->AddComponent(this);
}

Component::~Component() {
    mOwner->RemoveComponent(this);
}

void Component::Update(float deltaTime) {
}