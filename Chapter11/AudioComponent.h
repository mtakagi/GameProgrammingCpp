#pragma once

#include <string>
#include <vector>
#include "Component.h"

class Actor;
class SoundEvent;

class AudioComponent : public Component {
public:
    explicit AudioComponent(Actor* owner, int updateOrder = 200);
    ~AudioComponent() override;

    void Update(float deltaTime) override;
    void OnUpdateWorldTransform() override;

    SoundEvent PlayEvent(const std::string& name);
    void StopAllEvent();

private:
    std::vector<SoundEvent> mEvents2D;
    std::vector<SoundEvent> mEvents3D;
};
