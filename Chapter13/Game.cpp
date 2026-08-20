#include <algorithm>
#include <fstream>
#include <vector>
#include <rapidjson/document.h>
#include <sstream>
#include <SDL_ttf.h>
#include "Actor.h"
#include "Animation.h"
#include "AudioSystem.h"
#include "FollowActor.h"
#include "HUD.h"
#include "Game.h"
#include "Font.h"
#include "PauseMenu.h"
#include "PhysWorld.h"
#include "PlaneActor.h"
#include "PointLightComponent.h"
#include "Renderer.h"
#include "Skeleton.h"
#include "SpriteComponent.h"
#include "TargetActor.h"
#include "UIScreen.h"

Game::Game()
    : mRenderer(nullptr)
    , mAudioSystem(nullptr)
    , mPhysWorld(nullptr)
    , mHUD(nullptr)
    , mGameState(EGamePlay)
    , mTicksCount(0)
    , mIsRunning(true)
    , mUpdatingActors(false)
    , mFollowActor(nullptr)
    , mCrosshair(nullptr) {
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

    mAudioSystem = new AudioSystem(this);

    if (!mAudioSystem->Initialize()) {
        SDL_Log("オーディオシステムの初期化に失敗しました");
        mAudioSystem->Shutdown();
        delete mAudioSystem;
        mAudioSystem = nullptr;

        return false;
    }

    mPhysWorld = new PhysWorld(this);

    if (TTF_Init() != 0) {
        SDL_Log("SDL_ttf の初期化に失敗しました");

        return false;
    }

    LoadData();

    mTicksCount = SDL_GetTicks();

    return true;
}

void Game::RunLoop() {
    while (mGameState != EQuit) {
        ProcessInput();
        UpdateGame();
        GenerateOutput();
    }
}

void Game::AddPlane(PlaneActor *plane) {
    mPlanes.emplace_back(plane);
}

void Game::RemovePlane(PlaneActor *plane) {
    const auto iter = std::ranges::find(mPlanes, plane);
    mPlanes.erase(iter);
}

void Game::ProcessInput() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                mGameState = EQuit;
                break;
            case SDL_KEYDOWN:
                if (!event.key.repeat) {
                    if (mGameState == EGamePlay) {
                        HandleKeyPress(event.key.keysym.sym);
                    } else if (!mUIStack.empty()) {
                        mUIStack.back()->HandleKeyPress(event.key.keysym.sym);
                    }
                }
                break;
            case SDL_MOUSEBUTTONDOWN:
                if (mGameState == EGamePlay) {
                    HandleKeyPress(event.button.button);
                } else if (!mUIStack.empty()) {
                    mUIStack.back()->HandleKeyPress(event.button.button);
                }
                break;
            default:
                break;
        }
    }

    const auto state = SDL_GetKeyboardState(nullptr);

    if (mGameState == EGamePlay) {
        for (const auto& actor : mActors) {
            if (actor->GetState() == Actor::EActive) {
                actor->ProcessInput(state);
            }
        }
    } else if (!mUIStack.empty()) {
        mUIStack.back()->ProcessInput(state);
    }
}

void Game::HandleKeyPress(const int key) {
    switch (key) {
        case SDLK_ESCAPE:
            new PauseMenu(this);
            break;
        case '-': {
            auto volume = mAudioSystem->GetBusVolume("bus:/");
            volume = Math::Max(0.0f, volume - 0.1f);
            mAudioSystem->SetBusVolume("bus:/", volume);
            break;
        }
        case '=': {
            auto volume = mAudioSystem->GetBusVolume("bus:/");
            volume = Math::Min(1.0f, volume + 0.1f);
            mAudioSystem->SetBusVolume("bus:/", volume);
            break;
        }
        case '1': {
            LoadText("Assets/English.gptext");
            break;
        }
        case '2': {
            LoadText("Assets/Russian.gptext");
            break;
        }
        default:
            break;
    }
}

void Game::UpdateGame() {
    while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16)) {}
    float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;

    if (deltaTime > 0.05f) {
        deltaTime = 0.05f;
    }

    mTicksCount = SDL_GetTicks();

    if (mGameState == EGamePlay) {
        mUpdatingActors = true;

        for (const auto& actor : mActors) {
            actor->Update(deltaTime);
        }

        mUpdatingActors = false;

        for (auto& pending : mPendingActors) {
            pending->ComputeWorldTransform();
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

    mAudioSystem->Update(deltaTime);

    for (const auto& ui : mUIStack) {
        if (ui->GetState() == UIScreen::EActive) {
            ui->Update(deltaTime);
        }
    }

    auto iter = mUIStack.begin();

    while (iter != mUIStack.end()) {
        if ((*iter)->GetState() == UIScreen::EClosing) {
            delete *iter;
            iter = mUIStack.erase(iter);
        } else {
            ++iter;
        }
    }
}

void Game::GenerateOutput() const {
    mRenderer->Draw();
}

void Game::LoadData() {
    LoadText("Assets/English.gptext");

    Actor* a = nullptr;

    constexpr auto start = -1250.0f;
    constexpr auto size = 250.0f;

    for (auto i = 0; i < 10; i++) {
        for (auto j = 0; j < 10; j++) {
            a = new PlaneActor(this);
            auto pos = Vector3(start + static_cast<float>(i) * size, start + static_cast<float>(j) * size, -100.0f);
            a->SetPosition(pos);
            a = new Actor(this);
            pos.z += 100.0f;
            a->SetPosition(pos);
            auto p = new PointLightComponent(a);
            Vector3 color;
            switch ((i + j) % 5) {
                case 0:
                    color = Color::Green;
                    break;
                case 1:
                    color = Color::Blue;
                    break;
                case 2:
                    color = Color::Red;
                    break;
                case 3:
                    color = Color::Yellow;
                    break;
                case 4:
                    color = Color::LightPink;
                    break;
            }

            p->mDiffuseColor = color;
            p->mInnerRadius = 100.0f;
            p->mOuterRadius = 200.0f;
        }
    }

    Quaternion q(Vector3::UnitX, Math::PiOver2);

    for (auto i = 0; i < 10; i++) {
        a = new PlaneActor(this);
        a->SetPosition(Vector3(start + static_cast<float>(i) * size, start - size, 0.0f));
        a->SetRotation(q);

        a = new PlaneActor(this);
        a->SetPosition(Vector3(start + static_cast<float>(i) * size, -start + size, 0.0f));
        a->SetRotation(q);
    }

    mRenderer->SetAmbientLight(Vector3(0.4f, 0.4f, 0.4f));
    auto& dir = mRenderer->GetDirectionalLight();
    dir.mDirection = Vector3(0.0f, -70.0f, -70.0f);
    dir.mDiffuseColor = Vector3(0.78f, 0.78f, 0.78f);
    dir.mSpecColor = Vector3(0.8f, 0.8f, 0.8f);

    mHUD = new HUD(this);

    mMusicEvent = mAudioSystem->PlayEvent("event:/Music");

    SDL_SetRelativeMouseMode(SDL_TRUE);
    SDL_GetRelativeMouseState(nullptr, nullptr);

    mFollowActor = new FollowActor(this);

    a = new TargetActor(this);
    a->SetPosition(Vector3(1450.0f, 0.0f, 100.0f));
    a = new TargetActor(this);
    a->SetPosition(Vector3(1450.0f, 0.0f, 400.0f));
    a = new TargetActor(this);
    a->SetPosition(Vector3(1450.0f, -500.0f, 200.0f));
    a = new TargetActor(this);
    a->SetPosition(Vector3(1450.0f, 500.0f, 200.0f));
    a = new TargetActor(this);
    a->SetPosition(Vector3(0.0f, -1450.0f, 200.0f));
    a->SetRotation(Quaternion(Vector3::UnitZ, Math::PiOver2));
    a = new TargetActor(this);
    a->SetPosition(Vector3(0.0f, 1450.0f, 200.0f));
    a->SetRotation(Quaternion(Vector3::UnitZ, -Math::PiOver2));
}

void Game::UnloadData() {
    while (!mActors.empty()) {
        delete mActors.back();
    }

    while (!mUIStack.empty()) {
        delete mUIStack.back();
        mUIStack.pop_back();
    }

    if (mRenderer) {
        mRenderer->UnloadData();
    }
}

void Game::Shutdown() {
    UnloadData();
    TTF_Quit();
    delete mPhysWorld;

    if (mRenderer) {
        mRenderer->Shutdown();
    }

    if (mAudioSystem) {
        mAudioSystem->Shutdown();
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

void Game::PushUI(UIScreen *screen) {
    mUIStack.emplace_back(screen);
}

Font *Game::GetFont(const std::string &fileName) {
    const auto iter = mFonts.find(fileName);

    if (iter != mFonts.end()) {
        return iter->second;
    }

    auto font = new Font(this);

    if (font->Load(fileName)) {
        mFonts.emplace(fileName, font);
    } else {
        font->Unload();
        delete font;
        font = nullptr;
    }

    return font;
}

void Game::LoadText(const std::string &fileName) {
    mText.clear();
    std::ifstream file(fileName);

    if (!file.is_open()) {
        SDL_Log("テキストファイル %s が見つかりません", fileName.c_str());
        return;
    }

    std::stringstream fileStream;
    fileStream << file.rdbuf();
    std::string contents = fileStream.str();
    rapidjson::StringStream jsonStr(contents.c_str());
    rapidjson::Document doc;

    doc.ParseStream(jsonStr);

    if (!doc.IsObject()) {
        SDL_Log("テキストファイル %s は有効な JSON ではありません", fileName.c_str());

        return;
    }

    const rapidjson::Value& actions = doc["TextMap"];

    for (auto itr = actions.MemberBegin(); itr != actions.MemberEnd(); ++itr) {
        if (itr->name.IsString() && itr->value.IsString()) {
            mText.emplace(itr->name.GetString(), itr->value.GetString());
        }
    }
}

const std::string &Game::GetText(const std::string &key) {
    static std::string errorMsg("**KEY NOT FOUND**");

    const auto iter = mText.find(key);
    if (iter != mText.end()) {
        return iter->second;
    }

    return errorMsg;
}

Skeleton* Game::GetSkeleton(const std::string &fileName) {
    const auto iter = mSkeletons.find(fileName);

    if (iter != mSkeletons.end()) {
        return iter->second;
    }

    auto sk = new Skeleton();

    if (sk->Load(fileName)) {
        mSkeletons.emplace(fileName, sk);
    } else {
        delete sk;
        sk = nullptr;
    }

    return sk;
}

Animation* Game::GetAnimation(const std::string &fileName) {
    const auto iter = mAnims.find(fileName);

    if (iter != mAnims.end()) {
        return iter->second;
    }

    auto anim = new Animation();

    if (anim->Load(fileName)) {
        mAnims.emplace(fileName, anim);
    } else {
        delete anim;
        anim = nullptr;
    }

    return anim;
}