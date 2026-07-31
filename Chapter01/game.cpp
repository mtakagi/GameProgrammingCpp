#include "game.h"

constexpr int thickness = 15;
constexpr float paddleH = 100.0f;

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

    mRenderer = SDL_CreateRenderer(
        mWindow,
        -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );

    mPaddlePos.x = 10.0f;
    mPaddlePos.y = 768.0f/2.0f;
    mBallPos.x = 1024.0f/2.0f;
    mBallPos.y = 768.0f/2.0f;
    mBallVel.x = -200.0f;
    mBallVel.y = 235.0f;

    return true;
}

void Game::Shutdown() {
    SDL_DestroyWindow(mWindow);
    SDL_DestroyRenderer(mRenderer);
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

    mPaddleDir = 0;

    if (state[SDL_SCANCODE_W]) {
        mPaddleDir -= 1;
    }
    if (state[SDL_SCANCODE_S]) {
        mPaddleDir += 1;
    }
}

void Game::UpdateGame() {
    while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16)) {}
    float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;

    if (deltaTime > 0.05f) {
        deltaTime = 0.05f;
    }

    mTicksCount = SDL_GetTicks();

    if (mPaddleDir != 0) {
        mPaddlePos.y += mPaddleDir * 300.0f * deltaTime;

        if (mPaddlePos.y < paddleH / 2.0f + thickness) {
            mPaddlePos.y = paddleH / 2.0f + thickness;
        } else if (mPaddlePos.y > (768.0f - paddleH / 2.0f - thickness)) {
            mPaddlePos.y = 768.0f - paddleH / 2.0f - thickness;
        }
    }

    mBallPos.x += mBallVel.x * deltaTime;
    mBallPos.y += mBallVel.y * deltaTime;

    if (mBallPos.y <= thickness && mBallPos.y < 0.0f) {
        mBallVel.y *= -1;
    } else if (mBallPos.y >= (768 - thickness) && mBallPos.y > 0.0f) {
        mBallVel.y *= -1;
    }

    auto diff = mPaddlePos.y - mBallPos.y;
    diff = (diff > 0.0f) ? diff : -diff;

    if (diff <= paddleH / 2.0f && mBallPos.x <= 25.0f && mBallPos.x >= 20.0f && mBallVel.x < 0.0f) {
        mBallVel.x *= -1;
    } else if (mBallPos.x <= 0.0f) {
        mIsRunning = false;
    } else if (mBallPos.x >= (1024.0f - thickness) && mBallVel.x > 0.0f) {
        mBallVel.x *= -1;
    }
}

void Game::GenerateOutput() {
    SDL_SetRenderDrawColor(
        mRenderer,
        0,
        0,
        255,
        255
    );
    SDL_RenderClear(mRenderer);

    SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 255);
    SDL_Rect wall(0, 0, 1024, thickness);
    SDL_RenderFillRect(mRenderer, &wall);

    wall.y = 768 - thickness;

    SDL_RenderFillRect(mRenderer, &wall);

    wall.x = 1024 - thickness;
    wall.y = 0;
    wall.w = thickness;
    wall.h = 1024;

    SDL_RenderFillRect(mRenderer, &wall);

    const SDL_Rect paddle(
        static_cast<int>(mPaddlePos.x),
        static_cast<int>(mPaddlePos.y - paddleH / 2.0f),
        thickness,
        static_cast<int>(paddleH)
    );

    SDL_RenderFillRect(mRenderer, &paddle);

    const SDL_Rect ball(
        static_cast<int>(mBallPos.x - thickness / 2),
        static_cast<int>(mBallPos.y - thickness / 2),
        thickness,
        thickness
    );

    SDL_RenderFillRect(mRenderer, &ball);

    SDL_RenderPresent(mRenderer);
}
