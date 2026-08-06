#include <algorithm>
#include <GL/glew.h>
#include "Actor.h"
#include "Asteroid.h"
#include "Game.h"
#include "Shader.h"
#include "Ship.h"
#include "SpriteComponent.h"
#include "Texture.h"
#include "VertexArray.h"

Game::Game()
    : mSpriteShader(nullptr)
    , mSpriteVerts(nullptr)
    , mWindow(nullptr)
    , mContext(nullptr)
    , mTicksCount(0)
    , mIsRunning(true)
    , mUpdatingActors(true)
    , mShip(nullptr)
     {
}

bool Game::Initialize() {
    auto sdlResult = SDL_Init( SDL_INIT_VIDEO | SDL_INIT_AUDIO );

    if (sdlResult != 0) {
        SDL_Log("SDL を初期化できません: %s", SDL_GetError());
        return false;
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

    mWindow = SDL_CreateWindow(
        "Game Programming in C++ (第5章)",
        100,
        100,
        1024,
        768,
        SDL_WINDOW_OPENGL);

    if (!mWindow) {
        SDL_Log("ウィンドウの作成に失敗しました: %s", SDL_GetError());

        return false;
    }

    mContext = SDL_GL_CreateContext(mWindow);

    if (!mContext) {
        SDL_Log("OpenGL コンテキストの作成に失敗しました: %s", SDL_GetError());
        return false;
    }

    glewExperimental = GL_TRUE;

    if (glewInit() != GLEW_OK) {
        SDL_Log("GLEW の初期化に失敗しました");

        return false;
    }

    glGetError();

    if (!LoadShaders()) {
        SDL_Log("シェーダーのロードに失敗しました");

        return false;
    }

    CreateSpriteVerts();

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
    glClearColor(0.86f, 0.86f, 0.86f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    mSpriteShader->SetActive();
    mSpriteVerts->SetActive();

    for (auto& sprite : mSprites) {
        sprite->Draw(mSpriteShader);
    }

    SDL_GL_SwapWindow(mWindow);
}

bool Game::LoadShaders() {
    mSpriteShader = new Shader();

    if (!mSpriteShader->Load("Shaders/Sprite.vert", "Shaders/Sprite.frag")) {
        return false;
    }

    mSpriteShader->SetActive();

    const auto viewProj = Matrix4::CreateSimpleViewProj(1024.0f, 768.0f);

    mSpriteShader->SetMatrixUniform("uViewProj", viewProj);

    return true;
}

void Game::CreateSpriteVerts() {
    constexpr float vertices[] = {
        -0.5f,  0.5f, 0.0f, 0.0f, 0.0f,
         0.5f,  0.5f, 0.0f, 1.0f, 0.0f,
         0.5f, -0.5f, 0.0f, 1.0f, 1.0f,
        -0.5f, -0.5f, 0.0f, 0.0f, 1.0f,
    };

    constexpr unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0
    };

    mSpriteVerts = new VertexArray(vertices, 4, indices, 6);
}

void Game::LoadData() {
    mShip = new Ship(this);
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
        i.second->Unload();
        delete i.second;
    }
    mTextures.clear();
}

Texture* Game::GetTexture(const std::string& fileName) {
    Texture* tex = nullptr;
    auto iter = mTextures.find(fileName);

    if (iter != mTextures.end()) {
        tex = iter->second;
    } else {
        tex = new Texture();

        if (tex->Load(fileName)) {
            mTextures.emplace(fileName, tex);
        } else {
            delete tex;
            tex = nullptr;
        }
    }

    return tex;
}

void Game::AddAsteroid(Asteroid *asteroid) {
    mAsteroids.emplace_back(asteroid);
}

void Game::RemoveAsteroid(Asteroid *asteroid) {
    const auto iter = std::ranges::find(mAsteroids, asteroid);

    if (iter != mAsteroids.end()) {
        mAsteroids.erase(iter);
    }
}

void Game::Shutdown() {
    UnloadData();
    delete mSpriteVerts;
    mSpriteShader->Unload();
    delete mSpriteShader;
    SDL_GL_DeleteContext(mContext);
    SDL_DestroyWindow(mWindow);
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
    const auto iter = std::ranges::find(mSprites, sprite);
    mSprites.erase(iter);
}
