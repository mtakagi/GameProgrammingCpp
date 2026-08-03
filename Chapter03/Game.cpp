#include <algorithm>
#include <SDL_image.h>
#include "Game.h"
#include "Actor.h"
#include "Asteroid.h"
#include "BGSpriteComponent.h"
#include "Ship.h"
#include "SpriteComponent.h"

Game::Game()
    : mWindow(nullptr)
    , mRenderer(nullptr)
    , mTicksCount(0)
    , mIsRunning(true)
    , mUpdatingActors(true)
    , mShip(nullptr) {
}

bool Game::Initialize() {
    auto sdlResult = SDL_Init( SDL_INIT_VIDEO | SDL_INIT_AUDIO );

    if (sdlResult != 0) {
        SDL_Log("SDL を初期化できません: %s", SDL_GetError());
        return false;
    }

    mWindow = SDL_CreateWindow(
        "Game Programming in C++ (第2章)",
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

    if (IMG_Init(IMG_INIT_PNG) == 0) {
        SDL_Log("Unable to initialize SDL_image: %s", SDL_GetError());
    }

    LoadData();

    mTicksCount = SDL_GetTicks();

    return true;
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

    const auto state = SDL_GetKeyboardState(nullptr);

    if (state[SDL_SCANCODE_ESCAPE]) {
        mIsRunning = false;
    }

    mUpdatingActors = true;

    for (const auto& actor : mActors) {
        actor->ProcessInput(state);
    }

    mUpdatingActors = false;
}

void Game::UpdateGame() {
    while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16)) {}
    float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;

    if (deltaTime > 0.05f) {
        deltaTime = 0.05f;
    }

    mTicksCount = SDL_GetTicks();

    mUpdatingActors = true;

    for (const auto& actor : mActors) {
        actor->Update(deltaTime);
    }

    mUpdatingActors = false;

    for (auto& pending : mPendingActors) {
        mActors.emplace_back(pending);
    }

    mPendingActors.clear();

    std::vector<Actor *> deadActors;

    for (auto& actor : mActors) {
        if (actor->GetState() == Actor::EDead) {
            deadActors.emplace_back(actor);
        }
    }

    for (const auto& actor : deadActors) {
        delete actor;
    }
}

void Game::GenerateOutput() {
    SDL_SetRenderDrawColor(
        mRenderer,
        220,
        220,
        220,
        255
    );
    SDL_RenderClear(mRenderer);

    for (const auto& sprite : mSprites ) {
        sprite->Draw(mRenderer);
    }

    SDL_RenderPresent(mRenderer);
}

void Game::LoadData() {
    mShip = new Ship(this);
    mShip->SetPosition(Vector2(512.0f, 384.0f));
    mShip->SetRotation(Math::PiOver2);

    constexpr int numAsteroids = 20;

    for (auto i = 0; i < numAsteroids; i++) {
        new Asteroid(this);
    }
}

void Game::UnloadData() {
    while (!mActors.empty()) {
        delete mActors.back();
    }

    for (auto& i : mTextures) {
        SDL_DestroyTexture(i.second);
    }
    mTextures.clear();
}

SDL_Texture* Game::GetTexture(const std::string& fileName) {
    SDL_Texture* tex = nullptr;
    auto iter = mTextures.find(fileName);

    if (iter != mTextures.end()) {
        tex = iter->second;
    } else {
        SDL_Surface* surf = IMG_Load(fileName.c_str());

        if (!surf) {
            SDL_Log("Failed to load texture file: %s", fileName.c_str());
            return nullptr;
        }

        tex = SDL_CreateTextureFromSurface(mRenderer, surf);
        SDL_FreeSurface(surf);

        if (!tex) {
            SDL_Log("Failed to convert surface to texture: %s", fileName.c_str());
            return nullptr;
        }

        mTextures.emplace(fileName.c_str(), tex);
    }

    return tex;
}

void Game::AddAsteroid(Asteroid *asteroid) {
    mAsteroids.emplace_back(asteroid);
}

void Game::RemoveAsteroid(Asteroid *asteroid) {
    auto iter = std::ranges::find(mAsteroids, asteroid);

    if (iter != mAsteroids.end()) {
        mAsteroids.erase(iter);
    }
}

void Game::Shutdown() {
    UnloadData();
    IMG_Quit();
    SDL_DestroyWindow(mWindow);
    SDL_DestroyRenderer(mRenderer);
    SDL_Quit();
}

void Game::AddActor(Actor* actor) {
    if (mUpdatingActors) {
        mPendingActors.emplace_back(actor);
    } else {
        mActors.emplace_back(actor);
    }
}

void Game::RemoveActor(Actor* actor) {
    auto iter = std::ranges::find(mPendingActors, actor);

    if (iter != mPendingActors.end()) {
        std::iter_swap(iter, mPendingActors.end() - 1);
        mPendingActors.pop_back();
    }

    iter = std::ranges::find(mActors, actor);

    if (iter != mActors.end()) {
        std::iter_swap(iter, mActors.end() - 1);
        mActors.pop_back();
    }
}

void Game::AddSprite(SpriteComponent* sprite) {
    const auto myDrawOrder = sprite->GetDrawOrder();
    auto iter = mSprites.begin();
    for (; iter != mSprites.end(); ++iter) {
        if (myDrawOrder < (*iter)->GetDrawOrder()) {
            break;
        }
    }

    mSprites.insert(iter, sprite);
}

void Game::RemoveSprite(SpriteComponent* sprite) {
    auto iter = std::ranges::find(mSprites, sprite);
    mSprites.erase(iter);
}