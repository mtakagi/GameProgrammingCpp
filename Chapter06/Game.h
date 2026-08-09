#pragma once

#include <SDL.h>
#include <vector>

class Actor;
class CameraActor;
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

private:
    void ProcessInput();
    void UpdateGame();
    void GenerateOutput() const;
    void LoadData();
    void UnloadData() const;

    std::vector<Actor *> mActors;
    std::vector<Actor *> mPendingActors;

    Renderer *mRenderer;

    Uint32 mTicksCount;
    bool mIsRunning;
    bool mUpdatingActors;

    CameraActor *mCameraActor;
};