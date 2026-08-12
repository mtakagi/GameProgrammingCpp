#pragma once

#include <string>
#include <vector>

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
    float GetSpecPower() const { return mSpecPower; }

private:
    std::vector<Texture*> mTextures;
    VertexArray* mVertexArray;
    std::string mShaderName;
    float mRadius;
    float mSpecPower;
};
