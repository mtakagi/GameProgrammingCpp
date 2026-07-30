#pragma once
#include <SDL.h>

class Game {
public:
    Game() : mWindow(nullptr), mIsRunning(true) {};
    bool Initialize();
    void RunLoop();
    void Shutdown();
private:
    void ProcessInput();
    void UpdateGame();
    void GenerateOutput();

    SDL_Window *mWindow;
    bool mIsRunning;
};