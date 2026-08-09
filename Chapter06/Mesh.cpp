#include <fstream>
#include <sstream>
#include <SDL.h>
#include <rapidjson/document.h>
#include "Math.h"
#include "Mesh.h"
#include "Renderer.h"
#include "VertexArray.h"

Mesh::Mesh()
    : mVertexArray(nullptr)
    , mRadius(0.0f)
    , mSpecPower(100.0f) {
}

Mesh::~Mesh() = default;

bool Mesh::Load(const std::string &fileName, Renderer* renderer) {
    const std::ifstream file(fileName);

    if (!file.is_open()) {
        SDL_Log("メッシュファイル %s を開けません", fileName.c_str());

        return false;
    }

    std::stringstream fileStream;
    fileStream << file.rdbuf();
    const std::string contents = fileStream.str();
    rapidjson::StringStream jsonStr(contents.c_str());
    rapidjson::Document doc;
    doc.ParseStream(jsonStr);

    if (!doc.IsObject()) {
        SDL_Log("メッシュ %s は有効な JSON ではありません", fileName.c_str());

        return false;
    }

    const auto ver = doc["version"].GetInt();

    if (ver != 1) {
        SDL_Log("メッシュ %s のバージョン %d は非対応です", fileName.c_str(), ver);

        return false;
    }

    mShaderName = doc["shader"].GetString();

    size_t vertSize = 8;
    const rapidjson::Value& textures = doc["textures"];

    if (!textures.IsArray() || textures.Size() < 1) {
        SDL_Log("メッシュ %s に textures 配列がありません", fileName.c_str());

        return false;
    }

    mSpecPower = static_cast<float>(doc["specularPower"].GetDouble());

    for (rapidjson::SizeType i = 0; i < textures.Size(); i++) {
        const auto texName = textures[i].GetString();
        auto t = renderer->GetTexture(texName);

        if (t == nullptr) {
            t = renderer->GetTexture(texName);

            if (t == nullptr) {
                t = renderer->GetTexture("Assets/Default.png");
            }
        }

        mTextures.emplace_back(t);
    }

    const rapidjson::Value& vertsJson = doc["vertices"];

    if (!vertsJson.IsArray() || vertsJson.Size() < 1) {
        SDL_Log("メッシュ %s に vertices 配列がありません", fileName.c_str());

        return false;
    }

    std::vector<float> vertices;
    vertices.reserve(vertsJson.Size() * vertSize);
    mRadius = 0.0f;

    for (rapidjson::SizeType i = 0; i < vertsJson.Size(); i++) {
        const rapidjson::Value& vert = vertsJson[i];

        if (!vert.IsArray() || vert.Size() != 8) {
            SDL_Log("メッシュ %s の頂点 %u の要素数が %u です（位置 3 + 法線 3 + UV 2 = 8 が必要）",
            fileName.c_str(), i, vert.Size());

            return false;
        }

        Vector3 pos(vert[0].GetDouble(), vert[1].GetDouble(), vert[2].GetDouble());
        mRadius = Math::Max(mRadius, pos.LengthSq());

        for (rapidjson::SizeType j = 0; j < vert.Size(); j++) {
            vertices.emplace_back(static_cast<float>(vert[j].GetDouble()));
        }
    }

    mRadius = Math::Sqrt(mRadius);

    const rapidjson::Value& indJson = doc["indices"];

    if (!indJson.IsArray() || indJson.Size() < 1) {
        SDL_Log("メッシュ %s に indices 配列がありません", fileName.c_str());

        return false;
    }

    std::vector<unsigned int> indices;
    indices.reserve(indJson.Size() * 3);

    for (rapidjson::SizeType i = 0; i < indJson.Size(); i++) {
        const rapidjson::Value& ind = indJson[i];

        if (!ind.IsArray() || ind.Size() != 3) {
            SDL_Log("メッシュ %s の面 %u の要素数が %u です（三角形なので 3 が必要）",
            fileName.c_str(), i, ind.Size());

            return false;
        }

        indices.emplace_back(ind[0].GetUint());
        indices.emplace_back(ind[1].GetUint());
        indices.emplace_back(ind[2].GetUint());
    }

    mVertexArray = new VertexArray(vertices.data(), static_cast<unsigned>(vertices.size() / vertSize), indices.data(), static_cast<unsigned>(indices.size()));

    return true;
}

void Mesh::Unload() {
    delete mVertexArray;
    mVertexArray = nullptr;
}

Texture* Mesh::GetTexture(const size_t i) const {
    if (i < mTextures.size()) {
        return mTextures[i];
    }

    return nullptr;
}