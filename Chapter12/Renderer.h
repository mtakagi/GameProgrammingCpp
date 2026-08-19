#pragma once

#include <SDL.h>
#include <string>
#include <unordered_map>
#include <vector>
#include "Math.h"
#include "Mesh.h"

struct DirectionalLight {
    Vector3 mDirection;
    Vector3 mDiffuseColor;
    Vector3 mSpecColor;
};

class Game;
class Shader;
class SpriteComponent;
class Texture;
class Mesh;
class MeshComponent;
class SkeletalMeshComponent;

class Renderer {
public:
    explicit Renderer(Game *game);
    ~Renderer();

    bool Initialize(float screenWidth, float screenHeight);
    void Shutdown();
    void UnloadData();

    void Draw();

    void AddSprite(SpriteComponent* sprite);
    void RemoveSprite(SpriteComponent* sprite);

    void AddMeshComp(MeshComponent* mesh);
    void RemoveMeshComp(MeshComponent* mesh);

    Texture* GetTexture(const std::string& fileName);
    Mesh* GetMesh(const std::string& fileName);

    void SetViewMatrix(const Matrix4& view) { mView = view; }

    void SetAmbientLight(const Vector3& ambient) { mAmbientLight = ambient; }
    DirectionalLight& GetDirectionalLight() { return mDirLight; }

    [[nodiscard]]
    Vector3 Unproject(const Vector3& screenPoint) const;
    void GetScreenDirection(Vector3& outStart, Vector3& outDir) const;

    [[nodiscard]]
    float GetScreenWidth() const { return mScreenWidth; }
    [[nodiscard]]
    float GetScreenHeight() const { return mScreenHeight; }

private:
    bool LoadShaders();
    void CreateSpriteVerts();
    void SetLightUniforms(Shader* shader);

    std::unordered_map<std::string, Texture*> mTextures;
    std::unordered_map<std::string, Mesh*> mMeshes;

    std::vector<SpriteComponent*> mSprites;
    std::vector<MeshComponent*> mMeshComps;
    std::vector<SkeletalMeshComponent*> mSkeletalMeshes;

    Game* mGame;
    Shader* mSpriteShader;
    VertexArray* mSpriteVerts;

    Shader* mMeshShader;
    Shader* mSkinnedShader;

    Matrix4 mView;
    Matrix4 mProjection;

    float mScreenWidth;
    float mScreenHeight;

    Vector3 mAmbientLight;
    DirectionalLight mDirLight;

    SDL_Window* mWindow;
    SDL_GLContext mContext;
};

