#include "Actor.h"
#include "AudioComponent.h"
#include "AudioSystem.h"
#include "Game.h"
#include "SoundEvent.h"

AudioComponent::AudioComponent(Actor* owner, const int updateOrder)
    : Component(owner, updateOrder) {
}

AudioComponent::~AudioComponent() {
    StopAllEvent();
}

void AudioComponent::Update(const float deltaTime) {
    Component::Update(deltaTime);

    auto iter = mEvents2D.begin();

    while (iter != mEvents2D.end()) {
        if (!iter->IsValid()) {
            iter = mEvents2D.erase(iter);
        } else {
            ++iter;
        }
    }

    iter = mEvents3D.begin();

    while (iter != mEvents3D.end()) {
        if (!iter->IsValid()) {
            iter = mEvents3D.erase(iter);
        } else {
            ++iter;
        }
    }
}

void AudioComponent::OnUpdateWorldTransform() {
    const auto world = mOwner->GetWorldTransform();

    for (auto& event : mEvents3D) {
        if (event.IsValid()) {
            event.Set3DAttributes(world);
        }
    }
}

SoundEvent AudioComponent::PlayEvent(const std::string &name) {
    auto e = mOwner->GetGame()->GetAudioSystem()->PlayEvent(name);

    if (e.Is3D()) {
        mEvents3D.emplace_back(e);
        e.Set3DAttributes(mOwner->GetWorldTransform());
    } else {
        mEvents2D.emplace_back(e);
    }

    return e;
}

void AudioComponent::StopAllEvent() {
    for (auto& e : mEvents2D) {
        e.Stop();
    }

    for (auto& e : mEvents3D) {
        e.Stop();
    }

    mEvents2D.clear();
    mEvents3D.clear();
}