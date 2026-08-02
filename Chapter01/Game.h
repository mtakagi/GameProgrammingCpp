#pragma once
#include <SDL.h>

struct Vector2 {
    float x;
    float y;
};

class Game {
public:
    constexpr Game() :
    mWindow(nullptr),
    mRenderer(nullptr),
    mIsRunning(true),
    mPaddlePos(0.0f, 0.0f),
    mBallPos(0.0f, 0.0f),
    mTicksCount(0),
    mPaddleDir(0),
    mBallVel(-200.0f, 235.0f) {};

    [[nodiscard]]
    bool Initialize();
    void RunLoop();
    void Shutdown();
private:
    void ProcessInput();
    void UpdateGame();
    void GenerateOutput();

    SDL_Window *mWindow;
    SDL_Renderer *mRenderer;
    bool mIsRunning;

    Vector2 mPaddlePos;
    Vector2 mBallPos;

    Uint32 mTicksCount;

    int mPaddleDir;

    Vector2 mBallVel;
};