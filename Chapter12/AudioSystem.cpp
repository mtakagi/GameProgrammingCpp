#include <fmod_errors.h>
#include <fmod_studio.hpp>
#include <SDL.h>
#include <vector>
#include "AudioSystem.h"

unsigned int AudioSystem::sNextID = 0;

AudioSystem::AudioSystem(Game* game)
    : mGame(game)
    , mSystem(nullptr)
    , mCoreSystem(nullptr) {
}

AudioSystem::~AudioSystem() = default;

bool AudioSystem::Initialize() {
    FMOD::Debug_Initialize(
        FMOD_DEBUG_LEVEL_ERROR,
        FMOD_DEBUG_MODE_TTY
    );

    auto result = FMOD::Studio::System::create(&mSystem);

    if (result != FMOD_OK) {
        SDL_Log("FMOD システムの生成に失敗しました: %s", FMOD_ErrorString(result));

        return false;
    }

    result = mSystem->initialize(
        512,
        FMOD_STUDIO_INIT_NORMAL,
        FMOD_INIT_NORMAL,
        nullptr
    );

    if (result != FMOD_OK) {
        SDL_Log("FMOD システムの初期化に失敗しました: %s", FMOD_ErrorString(result));

        return false;
    }

    mSystem->getCoreSystem(&mCoreSystem);

    LoadBank("Assets/Master Bank.strings.bank");
    LoadBank("Assets/Master Bank.bank");

    return true;
}

void AudioSystem::Shutdown() {
    UnloadAllBanks();

    if (mSystem) {
        mSystem->release();
    }
}

void AudioSystem::LoadBank(const std::string &name) {
    if (mBanks.contains(name)) {
        return;
    }

    FMOD::Studio::Bank* bank = nullptr;
    const auto result = mSystem->loadBankFile(
        name.c_str(),
        FMOD_STUDIO_LOAD_BANK_NORMAL,
        &bank
    );

    constexpr auto maxPathLength = 512;

    if (result == FMOD_OK) {
        mBanks.emplace(name, bank);
        bank->loadSampleData();
        auto numEvents = 0;
        bank->getEventCount(&numEvents);

        if (numEvents > 0) {
            std::vector<FMOD::Studio::EventDescription*> events(numEvents);
            bank->getEventList(events.data(), numEvents, &numEvents);
            char eventName[maxPathLength];

            for (auto i = 0; i < numEvents; i++) {
                auto e = events[i];
                e->getPath(eventName, maxPathLength, nullptr);
                mEvents.emplace(eventName, e);
            }
        }

        auto numBuses = 0;
        bank->getBusCount(&numBuses);

        if (numBuses > 0) {
            std::vector<FMOD::Studio::Bus*> buses(numBuses);
            bank->getBusList(buses.data(), numBuses, &numBuses);
            char busName[512];

            for (auto i = 0; i < numBuses; i++) {
                auto bus = buses[i];
                bus->getPath(busName, 512, nullptr);
                mBuses.emplace(busName, bus);
            }
        }
    } else {
        SDL_Log("バンク %s の読み込みに失敗しました: %s", name.c_str(), FMOD_ErrorString(result));
    }
}

void AudioSystem::UnloadBank(const std::string& name) {
    const auto iter = mBanks.find(name);

    if (iter == mBanks.end()) {
        return;
    }

    const auto bank = iter->second;
    auto numEvents = 0;
    bank->getEventCount(&numEvents);

    if (numEvents > 0) {
        std::vector<FMOD::Studio::EventDescription*> events(numEvents);
        bank->getEventList(events.data(), numEvents, &numEvents);
        char eventName[512];

        for (auto i = 0; i < numEvents; i++) {
            const auto e = events[i];
            e->getPath(eventName, 512, nullptr);
            const auto eventi = mEvents.find(eventName);

            if (eventi != mEvents.end()) {
                mEvents.erase(eventi);
            }
        }
    }

    auto numBuses = 0;
    bank->getBusCount(&numBuses);

    if (numBuses > 0) {
        std::vector<FMOD::Studio::Bus*> buses(numBuses);
        bank->getBusList(buses.data(), numBuses, &numBuses);
        char busName[512];

        for (auto i = 0; i < numBuses; i++) {
            const auto bus = buses[i];
            bus->getPath(busName, 512, nullptr);
            const auto busi = mBuses.find(busName);

            if (busi != mBuses.end()) {
                mBuses.erase(busi);
            }
        }
    }

    bank->unloadSampleData();
    bank->unload();
    mBanks.erase(iter);
}

void AudioSystem::UnloadAllBanks() {
    for (const auto& iter : mBanks) {
        iter.second->unloadSampleData();
        iter.second->unload();
    }

    mBanks.clear();
    mEvents.clear();
    mBuses.clear();
}

SoundEvent AudioSystem::PlayEvent(const std::string& name) {
    unsigned int retID = 0;
    const auto iter = mEvents.find(name);

    if (iter != mEvents.end()) {
        FMOD::Studio::EventInstance* event = nullptr;
        iter->second->createInstance(&event);

        if (event) {
            event->start();
            sNextID++;
            retID = sNextID;
            mEventInstances.emplace(retID, event);
        }
    }

    return {this, retID};
}

void AudioSystem::Update(const float deltaTime) {
    std::vector<unsigned int> done;

    for (const auto& iter : mEventInstances) {
        const auto e = iter.second;
        FMOD_STUDIO_PLAYBACK_STATE state;
        e->getPlaybackState(&state);

        if (state == FMOD_STUDIO_PLAYBACK_STOPPED) {
            e->release();
            done.emplace_back(iter.first);
        }
    }

    for (const auto id : done) {
        mEventInstances.erase(id);
    }

    mSystem->update();
}

namespace {
    FMOD_VECTOR VecToFMOD(const Vector3& in) {
        FMOD_VECTOR v;
        v.x = in.x;
        v.y = in.y;
        v.z = in.z;

        return v;
    }
}

void AudioSystem::SetListener(const Matrix4 &viewMatrix) {
    auto invView = viewMatrix;
    invView.Invert();
    FMOD_3D_ATTRIBUTES listener;

    listener.position = VecToFMOD(invView.GetTranslation());
    listener.forward = VecToFMOD(invView.GetZAxis());
    listener.up = VecToFMOD(invView.GetYAxis());
    listener.velocity = {.x = 0.0f, .y = 0.0f, .z = 0.0f};

    mSystem->setListenerAttributes(0, &listener);
}

float AudioSystem::GetBusVolume(const std::string &name) const {
    auto retVal = 0.0f;
    const auto iter = mBuses.find(name);

    if (iter != mBuses.end()) {
        iter->second->getVolume(&retVal);
    }

    return retVal;
}

bool AudioSystem::GetBusPaused(const std::string &name) const {
    auto retVal = false;
    const auto iter = mBuses.find(name);

    if (iter != mBuses.end()) {
        iter->second->getPaused(&retVal);
    }

    return retVal;
}

void AudioSystem::SetBusVolume(const std::string &name, const float volume) {
    const auto iter = mBuses.find(name);

    if (iter != mBuses.end()) {
        iter->second->setVolume(volume);
    }
}

void AudioSystem::SetBusPaused(const std::string &name, const bool paused) {
    const auto iter = mBuses.find(name);

    if (iter != mBuses.end()) {
        iter->second->setPaused(paused);
    }
}

FMOD::Studio::EventInstance* AudioSystem::GetEventInstance(const unsigned int id) {
    FMOD::Studio::EventInstance* event = nullptr;
    const auto iter = mEventInstances.find(id);

    if (iter != mEventInstances.end()) {
        event = iter->second;
    }

    return event;
}