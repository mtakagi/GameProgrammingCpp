#include <algorithm>
#include <vector>
#include "Font.h"
#include "Game.h"
#include "Texture.h"

Font::Font(Game *game)
    : mGame(game) {
}

Font::~Font() = default;

bool Font::Load(const std::string &fileName) {
    const std::vector fontSizes = {
        8, 9,
        10, 11, 12, 14, 16, 18,
        20, 22, 24, 26, 28,
        30, 32, 34, 36, 38,
        40, 42, 44, 46, 48,
        52, 56,
        60, 64, 68,
        72
    };

    return std::ranges::all_of(fontSizes, [this, fileName](auto size) {
        auto font = TTF_OpenFont(fileName.c_str(), size);

        if (font == nullptr) {
            SDL_Log("フォント %s のサイズ %d の読み込みに失敗しました", fileName.c_str(), size);

            return false;
        }

        mFontData.emplace(size, font);

        return true;
    });
}

void Font::Unload() const {
    for (const auto& font : mFontData) {
        TTF_CloseFont(font.second);
    }
}

Texture *Font::RenderText(const std::string &texKey, const Vector3 &color, const int pointSize) {
    Texture* texture = nullptr;
    SDL_Color sdlColor;
    sdlColor.r = static_cast<Uint8>(color.x * 255);
    sdlColor.g = static_cast<Uint8>(color.y * 255);
    sdlColor.b = static_cast<Uint8>(color.z * 255);
    sdlColor.a = 255;

    const auto iter = mFontData.find(pointSize);

    if (iter != mFontData.end()) {
        const auto font = iter->second;
        const auto actualText = mGame->GetText(texKey);
        const auto surf = TTF_RenderUTF8_Blended(font, actualText.c_str(), sdlColor);

        if (surf != nullptr) {
            texture = new Texture();
            texture->CreateFromSurface(surf);
            SDL_FreeSurface(surf);
        }
    } else {
        SDL_Log("ポイントサイズ %d は非対応です", pointSize);
    }

    return texture;
}
