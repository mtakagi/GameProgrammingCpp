#include <SDL_log.h>
#include "Actor.h"
#include "AIComponent.h"
#include "AIState.h"

AIComponent::AIComponent(Actor *owner)
    : Component(owner)
    , mCurrentState(nullptr){
}

void AIComponent::RegisterState(AIState *state) {
    mStateMap.emplace(state->GetName(), state);
}

void AIComponent::Update(const float deltaTime) {
    if (mCurrentState) {
        mCurrentState->Update(deltaTime);
    }
}

void AIComponent::ChangeState(const std::string& name) {
    if (mCurrentState) {
        mCurrentState->OnExit();
    }

    auto iter = mStateMap.find(name);
    if (iter != mStateMap.end()) {
        mCurrentState = iter->second;
        mCurrentState->OnEnter();
    } else {
        SDL_Log("AIState %s の状態はありません", name.c_str());
        mCurrentState = nullptr;
    }
}