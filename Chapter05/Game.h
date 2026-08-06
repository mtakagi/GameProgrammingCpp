#pragma once

#include <SDL.h>
#include <string>
#include <unordered_map>
#include <vector>

class Actor;
class Asteroid;
class SpriteComponent;
class Shader;
class Ship;
class Texture;
class Vector2;
class VertexArray;

class Game {
public:
    Game();

    bool Initialize();
    void RunLoop();
    void Shutdown();

    void AddActor(Actor* actor);
    void RemoveActor(Actor* actor);

    void AddSprite(SpriteComponent* sprite);
    void RemoveSprite(SpriteComponent* sprite);

    Texture* GetTexture(const std::string& fileName);

    void AddAsteroid(Asteroid* asteroid);
    void RemoveAsteroid(Asteroid* asteroid);
    std::vector<Asteroid*>& GetAsteroids() { return mAsteroids; }
private:
    void ProcessInput();
    void UpdateGame();
    void GenerateOutput();
    void LoadData();
    void UnloadData();

    bool LoadShaders();
    void CreateSpriteVerts();

    std::unordered_map<std::string, Texture*> mTextures;

    std::vector<Actor *> mActors;
    std::vector<Actor *> mPendingActors;

    std::vector<SpriteComponent *> mSprites;

    Shader *mSpriteShader;
    VertexArray *mSpriteVerts;

    SDL_Window *mWindow;
    SDL_GLContext mContext;
    Uint32 mTicksCount;
    bool mIsRunning;
    bool mUpdatingActors;

    Ship* mShip;
    std::vector<Asteroid*> mAsteroids;
};