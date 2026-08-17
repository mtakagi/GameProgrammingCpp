#pragma once

#include <string>

class Texture {
public:
    Texture();
    ~Texture();

    bool Load(const std::string& fileName);
    void Unload() const;
    void CreateFromSurface(const SDL_Surface* surface);

    void SetActive() const;

    [[nodiscard]]
    int GetWidth() const { return mWidth; }
    [[nodiscard]]
    int GetHeight() const { return mHeight; }

private:
    unsigned int mTextureID;
    int mWidth;
    int mHeight;
};
