#pragma once

#include "Component.h"

class Actor;

class TargetComponent : public Component {
public:
    explicit TargetComponent(Actor* owner);
    ~TargetComponent() override;
};
