#include "game.h"

bool Game::Initialize() {
    auto sdlResult = SDL_Init( SDL_INIT_VIDEO );

    if (sdlResult != 0) {
        SDL_Log("SDL を初期化できません: %s", SDL_GetError());
        return false;
    }

    mWindow = SDL_CreateWindow(
        "Game Programming in C++ (第1章)",
        100,
        100,
        1024,
        768,
        0);

    if (!mWindow) {
        SDL_Log("ウィンドウの作成に失敗しました: %s", SDL_GetError());
        return false;
    }

    return true;
}

void Game::Shutdown() {
    SDL_DestroyWindow(mWindow);
    SDL_Quit();
}

void Game::RunLoop() {
    while (mIsRunning) {
        ProcessInput();
        UpdateGame();
        GenerateOutput();
    }
}

void Game::ProcessInput() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                mIsRunning = false;
                break;
        }
    }

    auto state = SDL_GetKeyboardState(nullptr);

    if (state[SDL_SCANCODE_ESCAPE]) {
        mIsRunning = false;
    }
}
void Game::UpdateGame() {}
void Game::GenerateOutput() {}
