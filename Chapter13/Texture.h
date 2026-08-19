#pragma once

#include <SDL.h>
#include <string>

class Texture {
public:
    Texture();
    ~Texture();

    bool Load(const std::string& fileName);
    void Unload() const;
    void CreateFromSurface(const SDL_Surface* surface);
    void CreateForRendering(int width, int height, unsigned int format);

    void SetActive() const;

    [[nodiscard]]
    int GetWidth() const { return mWidth; }
    [[nodiscard]]
    int GetHeight() const { return mHeight; }
    [[nodiscard]]
    unsigned int GetTextureID() const { return mTextureID; }

private:
    unsigned int mTextureID;
    int mWidth;
    int mHeight;
};
