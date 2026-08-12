#pragma once

#include <SDL.h>
#include <vector>
#include "SoundEvent.h"

class Actor;
class AudioSystem;
class FollowActor;
class FPSActor;
class OrbitActor;
class SplineActor;
class SpriteComponent;
class Renderer;

class Game {
public:
    Game();

    bool Initialize();
    void RunLoop();
    void Shutdown();

    void AddActor(Actor* actor);
    void RemoveActor(Actor* actor);

    [[nodiscard]]
    Renderer* GetRenderer() const { return mRenderer; }
    [[nodiscard]]
    AudioSystem* GetAudioSystem() const { return mAudioSystem; }

private:
    void ProcessInput();
    void HandleKeyPress(int key);
    void UpdateGame();
    void GenerateOutput() const;
    void LoadData();
    void UnloadData() const;
    void ChangeCamera(int mode);

    std::vector<Actor *> mActors;
    std::vector<Actor *> mPendingActors;

    Renderer *mRenderer;
    AudioSystem *mAudioSystem;

    Uint32 mTicksCount;
    bool mIsRunning;
    bool mUpdatingActors;

    FPSActor* mFPSActor;
    FollowActor* mFollowActor;
    OrbitActor* mOrbitActor;
    SplineActor* mSplineActor;
    Actor* mStartSphere;
    Actor* mEndSphere;
    SpriteComponent* mCrosshair;

    SoundEvent mMusicEvent;
    SoundEvent mReverbSnap;
};