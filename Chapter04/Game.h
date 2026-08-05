#pragma once

#include <SDL.h>
#include <string>
#include <unordered_map>
#include <vector>

class Actor;
class Enemy;
class Grid;
class SpriteComponent;
class Vector2;

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

    SDL_Texture* GetTexture(const std::string& fileName);

    [[nodiscard]]
    Grid* GetGrid() const { return mGrid; }
    std::vector<Enemy *>& GetEnemies() { return mEnemies; }
    [[nodiscard]]
    Enemy* GetNearestEnemy(const Vector2& pos) const;
private:
    void ProcessInput();
    void UpdateGame();
    void GenerateOutput();
    void LoadData();
    void UnloadData();

    std::unordered_map<std::string, SDL_Texture*> mTextures;

    std::vector<Actor *> mActors;
    std::vector<Actor *> mPendingActors;

    std::vector<SpriteComponent *> mSprites;

    SDL_Window *mWindow;
    SDL_Renderer *mRenderer;
    Uint32 mTicksCount;
    bool mIsRunning;
    bool mUpdatingActors;

    std::vector<Enemy *> mEnemies;
    Grid *mGrid;
    float mNextEnemy;
};