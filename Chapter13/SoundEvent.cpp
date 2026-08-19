#include <fmod_studio.hpp>
#include "AudioSystem.h"
#include "SoundEvent.h"

SoundEvent::SoundEvent(AudioSystem* system, const unsigned int id)
    : mSystem(system)
    , mID(id) {
}

SoundEvent::SoundEvent()
    : mSystem(nullptr)
    , mID(0) {
}

bool SoundEvent::IsValid() const {
    return (mSystem && mSystem->GetEventInstance(mID) != nullptr);
}

void SoundEvent::Restart() {
    const auto event = mSystem ? mSystem->GetEventInstance(mID) : nullptr;

    if (event) {
        event->start();
    }
}

void SoundEvent::Stop(const bool allowFadeOut) {
    const auto event = mSystem ? mSystem->GetEventInstance(mID) : nullptr;

    if (event) {
        const auto mode = allowFadeOut ? FMOD_STUDIO_STOP_ALLOWFADEOUT : FMOD_STUDIO_STOP_IMMEDIATE;
        event->stop(mode);
    }
}

void SoundEvent::SetPaused(const bool paused) {
    const auto event = mSystem ? mSystem->GetEventInstance(mID) : nullptr;

    if (event) {
        event->setPaused(paused);
    }
}

void SoundEvent::SetVolume(const float value) {
    const auto event = mSystem ? mSystem->GetEventInstance(mID) : nullptr;

    if (event) {
        event->setVolume(value);
    }
}

void SoundEvent::SetPitch(const float value) {
    const auto event = mSystem ? mSystem->GetEventInstance(mID) : nullptr;

    if (event) {
        event->setPitch(value);
    }
}

void SoundEvent::SetParameter(const std::string &name, float value) {
    const auto event = mSystem ? mSystem->GetEventInstance(mID) : nullptr;

    if (event) {
        event->setParameterByName(name.c_str(), value);
    }
}

bool SoundEvent::GetPaused() const {
    auto retVal = false;
    const auto event = mSystem ? mSystem->GetEventInstance(mID) : nullptr;

    if (event) {
        event->getPaused(&retVal);
    }

    return retVal;
}

float SoundEvent::GetVolume() const {
    auto retVal = 0.0f;
    const auto event = mSystem ? mSystem->GetEventInstance(mID) : nullptr;

    if (event) {
        event->getVolume(&retVal);
    }

    return retVal;
}

float SoundEvent::GetPitch() const {
    auto retVal = 0.0f;
    const auto event = mSystem ? mSystem->GetEventInstance(mID) : nullptr;

    if (event) {
        event->getPitch(&retVal);
    }

    return retVal;
}

float SoundEvent::GetParameter(const std::string &name) const {
    auto retVal = 0.0f;
    const auto event = mSystem ? mSystem->GetEventInstance(mID) : nullptr;

    if (event) {
        event->getParameterByName(name.c_str(), &retVal);
    }

    return retVal;
}

bool SoundEvent::Is3D() const {
    auto retVal = false;
    const auto event = mSystem ? mSystem->GetEventInstance(mID) : nullptr;

    if (event) {
        FMOD::Studio::EventDescription* ed = nullptr;
        event->getDescription(&ed);

        if (ed) {
            ed->is3D(&retVal);
        }
    }

    return retVal;
}

namespace {
    FMOD_VECTOR VecToFMOD(const Vector3& vec) {
        FMOD_VECTOR v;
        v.x = vec.x;
        v.y = vec.y;
        v.z = vec.z;

        return v;
    }
}

void SoundEvent::Set3DAttributes(const Matrix4 &worldTrans) {
    const auto event = mSystem ? mSystem->GetEventInstance(mID) : nullptr;

    if (event) {
        FMOD_3D_ATTRIBUTES attr;

        attr.position = VecToFMOD(worldTrans.GetTranslation());
        attr.forward = VecToFMOD(worldTrans.GetXAxis());
        attr.up = VecToFMOD(worldTrans.GetZAxis());
        attr.velocity = { .x = 0.0f, .y = 0.0f, .z = 0.0f };
        event->set3DAttributes(&attr);
    }
}
