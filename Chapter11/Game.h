#pragma once

#include <SDL.h>
#include <vector>
#include <unordered_map>
#include "SoundEvent.h"

class Actor;
class AudioSystem;
class Font;
class FPSActor;
class HUD;
class SpriteComponent;
class PhysWorld;
class PlaneActor;
class Renderer;
class UIScreen;

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
    [[nodiscard]]
    HUD* GetHUD() const { return mHUD; }

    [[nodiscard]]
    const std::vector<UIScreen*>& GetUIStack() const { return mUIStack; }
    void PushUI(UIScreen* screen);

    [[nodiscard]]
    FPSActor* GetPlayer() const { return mFPSActor; }

    enum GameState {
        EGamePlay,
        EPaused,
        EQuit,
    };

    [[nodiscard]]
    GameState GetState() const { return mGameState; }
    void SetState(const GameState state) { mGameState = state; }

    [[nodiscard]]
    Font* GetFont(const std::string& fileName);

    void LoadText(const std::string& fileName);
    [[nodiscard]]
    const std::string& GetText(const std::string& key);

    void AddPlane(PlaneActor* plane);
    void RemovePlane(PlaneActor* plane);
    std::vector<PlaneActor*>& GetPlanes() { return mPlanes; }

private:
    void ProcessInput();
    void HandleKeyPress(int key);
    void UpdateGame();
    void GenerateOutput() const;
    void LoadData();
    void UnloadData();

    std::vector<Actor *> mActors;
    std::vector<Actor *> mPendingActors;

    std::vector<UIScreen*> mUIStack;
    std::unordered_map<std::string, Font*> mFonts;

    std::unordered_map<std::string, std::string> mText;

    Renderer *mRenderer;
    AudioSystem *mAudioSystem;
    PhysWorld *mPhysWorld;
    HUD *mHUD;

    GameState mGameState;

    Uint32 mTicksCount;
    bool mIsRunning;
    bool mUpdatingActors;

    std::vector<PlaneActor*> mPlanes;
    FPSActor* mFPSActor;
    SpriteComponent* mCrosshair;

    SoundEvent mMusicEvent;
};