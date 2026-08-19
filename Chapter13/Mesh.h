#pragma once

#include <string>
#include <vector>
#include "Collision.h"

class Renderer;
class Texture;
class VertexArray;

class Mesh {
public:
    Mesh();
    ~Mesh();

    bool Load(const std::string& fileName, Renderer* renderer);
    void Unload();

    [[nodiscard]]
    VertexArray* GetVertexArray() const { return mVertexArray; }
    [[nodiscard]]
    Texture* GetTexture(size_t i) const;

    [[nodiscard]]
    const std::string& GetShaderName() const { return mShaderName; }
    [[nodiscard]]
    float GetRadius() const { return mRadius; }
    [[nodiscard]]
    const AABB& GetBox() const { return mBox; }
    [[nodiscard]]
    float GetSpecPower() const { return mSpecPower; }

private:
    AABB mBox;
    std::vector<Texture*> mTextures;
    VertexArray* mVertexArray;
    std::string mShaderName;
    float mRadius;
    float mSpecPower;
};
