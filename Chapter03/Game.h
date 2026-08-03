#pragma once

#include <SDL.h>
#include <string>
#include <unordered_map>
#include <vector>
#include "Asteroid.h"

class Actor;
class SpriteComponent;
class Ship;
class Asteroid;

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

    void AddAsteroid(Asteroid* asteroid);
    void RemoveAsteroid(Asteroid* asteroid);
    std::vector<Asteroid*>& GetAsteroids() { return mAsteroids; }
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

    Ship* mShip;
    std::vector<Asteroid*> mAsteroids;
};