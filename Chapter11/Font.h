#pragma once

#include <string>
#include <unordered_map>
#include <SDL_ttf.h>
#include "Math.h"

class Game;
class Texture;

class Font {
public:
    explicit Font(Game* game);
    ~Font();

    bool Load(const std::string& fileName);
    void Unload() const;

    Texture* RenderText(const std::string& texKey, const Vector3& color = Color::White, int pointSize = 30);

private:
    std::unordered_map<int, TTF_Font*> mFontData;
    Game* mGame;
};
