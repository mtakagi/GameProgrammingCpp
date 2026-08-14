#pragma once

#include <string>
#include <unordered_map>
#include "SoundEvent.h"
#include "Math.h"

namespace FMOD {
    class System;

    namespace Studio {
        class Bank;
        class EventDescription;
        class EventInstance;
        class System;
        class Bus;
    }
}

class Game;

class AudioSystem {
public:
    explicit AudioSystem(Game* game);
    ~AudioSystem();

    bool Initialize();
    void Shutdown();

    void LoadBank(const std::string& name);
    void UnloadBank(const std::string& name);
    void UnloadAllBanks();

    SoundEvent PlayEvent(const std::string& name);

    void Update(float deltaTime);

    void SetListener(const Matrix4& viewMatrix);
    [[nodiscard]]
    float GetBusVolume(const std::string& name) const;
    [[nodiscard]]
    bool GetBusPaused(const std::string& name) const;
    void SetBusVolume(const std::string& name, float volume);
    void SetBusPaused(const std::string& name, bool paused);

protected:
    friend class SoundEvent;
    FMOD::Studio::EventInstance* GetEventInstance(unsigned int id);

private:
    static unsigned int sNextID;

    Game *mGame;

    std::unordered_map<std::string, FMOD::Studio::Bank*> mBanks;
    std::unordered_map<std::string, FMOD::Studio::EventDescription*> mEvents;
    std::unordered_map<unsigned int, FMOD::Studio::EventInstance*> mEventInstances;
    std::unordered_map<std::string, FMOD::Studio::Bus*> mBuses;

    FMOD::Studio::System* mSystem;
    FMOD::System* mCoreSystem;
};
