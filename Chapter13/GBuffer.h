#pragma once

#include <vector>

class Texture;

class GBuffer {
public:
    enum Type {
        EDiffuse,
        ENormal,
        EWorldPos,
        NUM_GBUFFER_TEXTURES
    };

    GBuffer();
    ~GBuffer();

    bool Create(int width, int height);
    void Destroy();

    Texture* GetTexture(Type type);
    [[nodiscard]]
    unsigned int GetBufferID() const { return mBufferID; }
    void SetTexturesActive();

private:
    std::vector<Texture*> mTextures;
    unsigned int mBufferID;
};
