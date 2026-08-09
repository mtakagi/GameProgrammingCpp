#include <algorithm>
#include <GL/glew.h>
#include <string>
#include "Game.h"
#include "Renderer.h"
#include "Shader.h"
#include "MeshComponent.h"
#include "SpriteComponent.h"
#include "Texture.h"
#include "VertexArray.h"

Renderer::Renderer(Game* game)
    : mGame(game)
    , mSpriteShader(nullptr)
    , mSpriteVerts(nullptr)
    , mMeshShader(nullptr)
    , mScreenWidth(0.0f)
    , mScreenHeight(0.0f)
    , mAmbientLight()
    , mDirLight()
    , mWindow(nullptr)
    , mContext(nullptr) {
}

Renderer::~Renderer() = default;

bool Renderer::Initialize(const float screenWidth, const float screenHeight) {
    mScreenWidth = screenWidth;
    mScreenHeight = screenHeight;

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

    mWindow = SDL_CreateWindow(
    "Game Programming in C++ (第6章)",
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

    return true;
}

void Renderer::Shutdown() {
    delete mSpriteVerts;
    mSpriteShader->Unload();
    delete mSpriteShader;
    mMeshShader->Unload();
    delete mMeshShader;
    SDL_GL_DeleteContext(mContext);
    SDL_DestroyWindow(mWindow);
}

void Renderer::UnloadData() {
    for (auto& i : mTextures) {
        i.second->Unload();
        delete i.second;
    }

    mTextures.clear();

    for (auto& i : mMeshes) {
        i.second->Unload();
        delete i.second;
    }

    mMeshes.clear();
}

void Renderer::Draw() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);

    mMeshShader->SetActive();
    mMeshShader->SetMatrixUniform("uViewProj", mView * mProjection);

    SetLightUniforms(mMeshShader);

    for (auto& mc : mMeshComps) {
        mc->Draw(mMeshShader);
    }

    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
    glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);

    mSpriteShader->SetActive();
    mSpriteVerts->SetActive();

    for (const auto& sprite : mSprites) {
        sprite->Draw(mSpriteShader);
    }

    SDL_GL_SwapWindow(mWindow);
}

void Renderer::AddSprite(SpriteComponent *sprite) {
    const auto myDrawOrder = sprite->GetDrawOrder();
    auto iter = mSprites.begin();

    for (; iter != mSprites.end(); ++iter) {
        if (myDrawOrder < (*iter)->GetDrawOrder()) {
            break;
        }
    }

    mSprites.insert(iter, sprite);
}

void Renderer::RemoveSprite(SpriteComponent *sprite) {
    const auto iter = std::ranges::find(mSprites, sprite);
    mSprites.erase(iter);
}

void Renderer::AddMeshComp(MeshComponent *mesh) {
    mMeshComps.emplace_back(mesh);
}

void Renderer::RemoveMeshComp(MeshComponent *mesh) {
    const auto iter = std::ranges::find(mMeshComps, mesh);
    mMeshComps.erase(iter);
}

Texture* Renderer::GetTexture(const std::string& fileName) {
    Texture* tex = nullptr;
    const auto iter = mTextures.find(fileName);

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

Mesh* Renderer::GetMesh(const std::string& fileName) {
    Mesh* mesh = nullptr;
    const auto iter = mMeshes.find(fileName);

    if (iter != mMeshes.end()) {
        mesh = iter->second;
    } else {
        mesh = new Mesh();

        if (mesh->Load(fileName, this)) {
            mMeshes.emplace(fileName, mesh);
        } else {
            delete mesh;
            mesh = nullptr;
        }
    }

    return mesh;
}

bool Renderer::LoadShaders() {
    mSpriteShader = new Shader();

    if (!mSpriteShader->Load("Shaders/Sprite.vert", "Shaders/Sprite.frag")) {
        return false;
    }

    mSpriteShader->SetActive();
    const auto viewProj = Matrix4::CreateSimpleViewProj(mScreenWidth, mScreenHeight);
    mSpriteShader->SetMatrixUniform("uViewProj", viewProj);

    mMeshShader = new Shader();

    if (!mMeshShader->Load("Shaders/Phong.vert", "Shaders/Phong.frag")) {
        return false;
    }

    mMeshShader->SetActive();
    mView = Matrix4::CreateLookAt(Vector3::Zero, Vector3::UnitX, Vector3::UnitZ);
    mProjection = Matrix4::CreatePerspectiveFOV(Math::ToRadians(70.0f), mScreenWidth, mScreenHeight, 25.0f, 10000.0f);
    mMeshShader->SetMatrixUniform("uViewProj", mView * mProjection);

    return true;
}

void Renderer::CreateSpriteVerts() {
    constexpr float vertices[] = {
        -0.5f,  0.5f, 0.0f,  0.0f, 0.0f, 0.0f,  0.0f, 0.0f,
         0.5f,  0.5f, 0.0f,  0.0f, 0.0f, 0.0f,  1.0f, 0.0f,
         0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 0.0f,  1.0f, 1.0f,
        -0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 0.0f,  0.0f, 1.0f,
    };

    constexpr unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0
    };

    mSpriteVerts = new VertexArray(vertices, 4, indices, 6);
}

void Renderer::SetLightUniforms(Shader *shader) {
    auto invView = mView;
    invView.Invert();
    shader->SetVectorUniform("uCameraPos", invView.GetTranslation());
    shader->SetVectorUniform("uAmbientLight", mAmbientLight);
    shader->SetVectorUniform("uDirLight.mDirection", mDirLight.mDirection);
    shader->SetVectorUniform("uDirLight.mDiffuseColor", mDirLight.mDiffuseColor);
    shader->SetVectorUniform("uDirLight.mSpecColor", mDirLight.mSpecColor);
}
