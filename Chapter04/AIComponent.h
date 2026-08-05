#pragma once

#include <string>
#include <unordered_map>
#include "Component.h"

class Actor;
class AIState;

class AIComponent : public Component {
public:
    explicit AIComponent(Actor* owner);

    void Update(float deltaTime) override;
    void ChangeState(const std::string& name);
    void RegisterState(AIState* state);

private:
    std::unordered_map<std::string, AIState*> mStateMap;
    AIState* mCurrentState;
};

