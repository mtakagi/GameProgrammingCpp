#pragma once

#include <SDL.h>
#include <vector>
#include "SoundEvent.h"

class Actor;
class AudioSystem;
class FPSActor;
class SpriteComponent;
class PhysWorld;
class PlaneActor;
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
    [[nodiscard]]
    PhysWorld* GetPhysWorld() const { return mPhysWorld; }

    void AddPlane(PlaneActor* plane);
    void RemovePlane(PlaneActor* plane);
    std::vector<PlaneActor*>& GetPlanes() { return mPlanes; }

private:
    void ProcessInput();
    void HandleKeyPress(int key);
    void UpdateGame();
    void GenerateOutput() const;
    void LoadData();
    void UnloadData() const;

    std::vector<Actor *> mActors;
    std::vector<Actor *> mPendingActors;

    Renderer *mRenderer;
    AudioSystem *mAudioSystem;
    PhysWorld *mPhysWorld;

    Uint32 mTicksCount;
    bool mIsRunning;
    bool mUpdatingActors;

    std::vector<PlaneActor*> mPlanes;
    FPSActor* mFPSActor;
    SpriteComponent* mCrosshair;

    SoundEvent mMusicEvent;
};