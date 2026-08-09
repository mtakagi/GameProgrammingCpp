#include <algorithm>
#include "Actor.h"
#include "CameraActor.h"
#include "Game.h"
#include "MeshComponent.h"
#include "PlaneActor.h"
#include "Renderer.h"
#include "SpriteComponent.h"

Game::Game()
    : mRenderer(nullptr)
    , mTicksCount(0)
    , mIsRunning(true)
    , mUpdatingActors(true)
    , mCameraActor(nullptr)
     {
}

bool Game::Initialize() {
    auto sdlResult = SDL_Init( SDL_INIT_VIDEO | SDL_INIT_AUDIO );

    if (sdlResult != 0) {
        SDL_Log("SDL を初期化できません: %s", SDL_GetError());
        return false;
    }

    mRenderer = new Renderer(this);

    if (!mRenderer->Initialize(1024.0f, 768.0f)) {
        SDL_Log("レンダラーの初期化に失敗しました");
        delete mRenderer;
        mRenderer = nullptr;

        return false;
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

    for (const auto& actor : mActors) {
        actor->ProcessInput(state);
    }
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

void Game::GenerateOutput() const {
    mRenderer->Draw();
}

void Game::LoadData() {
    auto a = new Actor(this);

    a->SetPosition(Vector3(200.0f, 75.0f, 0.0f));
    a->SetScale(100.0f);

    Quaternion q(Vector3::UnitY, -Math::PiOver2);

    q = Quaternion::Concatenate(q, Quaternion(Vector3::UnitZ, Math::Pi + Math::Pi / 4.0f));
    a->SetRotation(q);

    auto mc = new MeshComponent(a);

    mc->SetMesh(mRenderer->GetMesh("Assets/Cube.gpmesh"));

    a = new Actor(this);
    a->SetPosition(Vector3(200.0f, -75.0f, 0.0f));
    a->SetScale(3.0f);

    mc = new MeshComponent(a);
    mc->SetMesh(mRenderer->GetMesh("Assets/Sphere.gpmesh"));

    constexpr auto start = -1250.0f;
    constexpr auto size = 250.0f;

    for (auto i = 0; i < 10; i++) {
        for (auto j = 0; j < 10; j++) {
            a = new PlaneActor(this);
            a->SetPosition(Vector3(start + static_cast<float>(i) * size, start + static_cast<float>(j) * size, -100.0f));
        }
    }

    q = Quaternion(Vector3::UnitX, Math::PiOver2);

    for (auto i = 0; i < 10; i++) {
        a = new PlaneActor(this);
        a->SetPosition(Vector3(start + static_cast<float>(i) * size, start - size, 0.0f));
        a->SetRotation(q);

        a = new PlaneActor(this);
        a->SetPosition(Vector3(start + static_cast<float>(i) * size, -start + size, 0.0f));
        a->SetRotation(q);
    }

    q = Quaternion::Concatenate(q, Quaternion(Vector3::UnitZ, Math::PiOver2));

    for (auto i = 0; i < 10; i++) {
        a = new PlaneActor(this);
        a->SetPosition(Vector3(start - size, start + static_cast<float>(i) * size, 0.0f));
        a->SetRotation(q);

        a = new PlaneActor(this);
        a->SetPosition(Vector3(-start + size, start + static_cast<float>(i) * size, 0.0f));
        a->SetRotation(q);
    }

    mRenderer->SetAmbientLight(Vector3(0.2f, 0.2f, 0.2f));

    auto& dir = mRenderer->GetDirectionalLight();
    dir.mDirection = Vector3(0.0f, -0.707f, -0.707f);
    dir.mDiffuseColor = Vector3(0.78f, 0.88f, 1.0f);
    dir.mSpecColor = Vector3(0.8f, 0.8f, 0.8f);

    mCameraActor = new CameraActor(this);

    a = new Actor(this);
    a->SetPosition(Vector3(-350.0f, -350.0f, 0.0f));
    auto sc = new SpriteComponent(a);
    sc->SetTexture(mRenderer->GetTexture("Assets/HealthBar.png"));

    a = new Actor(this);
    a->SetPosition(Vector3(375.0f, -275.0f, 0.0f));
    a->SetScale(0.75f);
    sc = new SpriteComponent(a);
    sc->SetTexture(mRenderer->GetTexture("Assets/Radar.png"));
}

void Game::UnloadData() const {
    while (!mActors.empty()) {
        delete mActors.back();
    }

    if (mRenderer) {
        mRenderer->UnloadData();
    }
}

void Game::Shutdown() {
    UnloadData();

    if (mRenderer) {
        mRenderer->Shutdown();
    }

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
