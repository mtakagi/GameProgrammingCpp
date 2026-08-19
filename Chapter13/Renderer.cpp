#include <algorithm>
#include <GL/glew.h>
#include <string>
#include "Game.h"
#include "GBuffer.h"
#include "Renderer.h"
#include "Shader.h"
#include "MeshComponent.h"
#include "SkeletalMeshComponent.h"
#include "SpriteComponent.h"
#include "Texture.h"
#include "UIScreen.h"
#include "VertexArray.h"

Renderer::Renderer(Game* game)
    : mGame(game)
    , mSpriteShader(nullptr)
    , mSpriteVerts(nullptr)
    , mMeshShader(nullptr)
    , mSkinnedShader(nullptr)
    , mScreenWidth(0.0f)
    , mScreenHeight(0.0f)
    , mMirrorBuffer(0)
    , mMirrorTexture(nullptr)
    , mGBuffer(nullptr)
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
    "Game Programming in C++ (第13章)",
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

    if (!CreateMirrorTarget()) {
        SDL_Log("");

        return false;
    }

    mGBuffer = new GBuffer();
    const auto width = static_cast<int>(mScreenWidth);
    const auto height = static_cast<int>(mScreenHeight);

    if (!mGBuffer->Create(width, height)) {
        SDL_Log("Gバッファの作成に失敗しました");

        return false;
    }

    return true;
}

void Renderer::Shutdown() {
    if (mMirrorTexture != nullptr) {
        glDeleteFramebuffers(1, &mMirrorBuffer);
        mMirrorTexture->Unload();
        delete mMirrorTexture;
    }

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
    Draw3DScene(mMirrorBuffer, mMirrorView, mProjection, 0.25f);
    Draw3DScene(mGBuffer->GetBufferID(), mView, mProjection, 1.0f, false);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
    glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);

    mSpriteShader->SetActive();
    mSpriteVerts->SetActive();

    for (const auto& sprite : mSprites) {
        if (sprite->GetVisible()) {
            sprite->Draw(mSpriteShader);
        }
    }

    for (const auto& ui : mGame->GetUIStack()) {
        ui->Draw(mSpriteShader);
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
    if (mesh->GetIsSkeletal()) {
        auto sk = static_cast<SkeletalMeshComponent*>(mesh);
        mSkeletalMeshes.emplace_back(sk);
    } else {
        mMeshComps.emplace_back(mesh);
    }
}

void Renderer::RemoveMeshComp(MeshComponent *mesh) {
    if (mesh->GetIsSkeletal()) {
        const auto sk = static_cast<SkeletalMeshComponent*>(mesh);
        const auto iter = std::ranges::find(mSkeletalMeshes, sk);
        mSkeletalMeshes.erase(iter);
    } else {
        const auto iter = std::ranges::find(mMeshComps, mesh);
        mMeshComps.erase(iter);
    }
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

void Renderer::Draw3DScene(unsigned int framebuffer, const Matrix4 &view, const Matrix4 &proj, float viewPortScale, bool lit) {
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

    glViewport(0, 0, static_cast<int>(mScreenWidth * viewPortScale), static_cast<int>(mScreenHeight * viewPortScale));

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);

    mMeshShader->SetActive();
    mMeshShader->SetMatrixUniform("uViewProj", view * proj);

    if (lit) {
        SetLightUniforms(mMeshShader, view);
    }

    for (const auto& mc : mMeshComps) {
        if (mc->GetVisible()) {
            mc->Draw(mMeshShader);
        }
    }

    mSkinnedShader->SetActive();
    mSkinnedShader->SetMatrixUniform("uViewProj", view * proj);

    if (lit) {
        SetLightUniforms(mSkinnedShader, view);
    }

    for (const auto& sk : mSkeletalMeshes) {
        if (sk->GetVisible()) {
            sk->Draw(mSkinnedShader);
        }
    }
}

bool Renderer::CreateMirrorTarget() {
    const auto width = static_cast<int>(mScreenWidth) / 4;
    const auto height = static_cast<int>(mScreenHeight) / 4;

    glGenFramebuffers(1, &mMirrorBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, mMirrorBuffer);

    mMirrorTexture = new Texture();
    mMirrorTexture->CreateForRendering(width, height, GL_RGB);

    GLuint depthBuffer;
    glGenRenderbuffers(1, &depthBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);

    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, mMirrorTexture->GetTextureID(), 0);

    constexpr GLenum drawBuffers[] = { GL_COLOR_ATTACHMENT0 };
    glDrawBuffers(1, drawBuffers);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        glDeleteFramebuffers(1, &mMirrorBuffer);
        mMirrorTexture->Unload();
        delete mMirrorTexture;
        mMirrorTexture = nullptr;

        return false;
    }

    return true;
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

    if (!mMeshShader->Load("Shaders/Phong.vert", "Shaders/GBufferWrite.frag")) {
        return false;
    }

    mMeshShader->SetActive();
    mView = Matrix4::CreateLookAt(Vector3::Zero, Vector3::UnitX, Vector3::UnitZ);
    mProjection = Matrix4::CreatePerspectiveFOV(Math::ToRadians(70.0f), mScreenWidth, mScreenHeight, 25.0f, 10000.0f);
    mMeshShader->SetMatrixUniform("uViewProj", mView * mProjection);

    mSkinnedShader = new Shader();

    if (!mSkinnedShader->Load("Shaders/Skinned.vert", "Shaders/GBufferWrite.frag")) {
        return false;
    }

    mSkinnedShader->SetActive();
    mView = Matrix4::CreateLookAt(Vector3::Zero, Vector3::UnitX, Vector3::UnitZ);
    mProjection = Matrix4::CreatePerspectiveFOV(Math::ToRadians(70.0f), mScreenWidth, mScreenHeight, 10.0f, 10000.0f);
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

    mSpriteVerts = new VertexArray(vertices, 4, VertexArray::PosNormTex, indices, 6);
}

void Renderer::SetLightUniforms(Shader *shader, const Matrix4& view) {
    auto invView = view;
    invView.Invert();
    shader->SetVectorUniform("uCameraPos", invView.GetTranslation());
    shader->SetVectorUniform("uAmbientLight", mAmbientLight);
    shader->SetVectorUniform("uDirLight.mDirection", mDirLight.mDirection);
    shader->SetVectorUniform("uDirLight.mDiffuseColor", mDirLight.mDiffuseColor);
    shader->SetVectorUniform("uDirLight.mSpecColor", mDirLight.mSpecColor);
}

Vector3 Renderer::Unproject(const Vector3 &screenPoint) const {
    auto deviceCoord = screenPoint;
    deviceCoord.x /= mScreenWidth * 0.5f;
    deviceCoord.y /= mScreenHeight * 0.5f;

    auto unprojection = mView * mProjection;
    unprojection.Invert();

    return Vector3::TransformWithPerspDiv(deviceCoord, unprojection);
}

void Renderer::GetScreenDirection(Vector3 &outStart, Vector3 &outDir) const {
    Vector3 screenPoint(0.0f, 0.0f, 0.0f);
    outStart = Unproject(screenPoint);
    screenPoint.z = 0.9f;
    const auto end = Unproject(screenPoint);
    outDir = end - outStart;
    outDir.Normalize();
}

