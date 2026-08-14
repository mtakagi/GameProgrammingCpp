#pragma once

#include <string>
#include "Math.h"

class AudioSystem;

class SoundEvent {
public:
    SoundEvent();

    [[nodiscard]]
    bool IsValid() const;
    void Restart();
    void Stop(bool allowFadeOut = true);
    void SetPaused(bool paused);
    void SetVolume(float value);
    void SetPitch(float value);
    void SetParameter(const std::string& name, float value);
    [[nodiscard]]
    bool GetPaused() const;
    [[nodiscard]]
    float GetVolume() const;
    [[nodiscard]]
    float GetPitch() const;
    [[nodiscard]]
    float GetParameter(const std::string& name) const;
    [[nodiscard]]
    bool Is3D() const;
    void Set3DAttributes(const Matrix4& worldTrans);

protected:
    friend class AudioSystem;
    SoundEvent(AudioSystem* system, unsigned int id);

private:
    AudioSystem* mSystem;
    unsigned int mID;
};
