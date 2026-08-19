#include <fstream>
#include <sstream>
#include <SDL.h>
#include <rapidjson/document.h>
#include "Skeleton.h"
#include "MatrixPalette.h"

bool Skeleton::Load(const std::string &fileName) {
    std::ifstream file(fileName);

    if (!file.is_open()) {
        SDL_Log("スケルトンファイル %s を開けません", fileName.c_str());

        return false;
    }

    std::stringstream fileStream;
    fileStream << file.rdbuf();
    std::string contents = fileStream.str();
    rapidjson::StringStream jsonStr(contents.c_str());
    rapidjson::Document doc;

    doc.ParseStream(jsonStr);

    if (!doc.IsObject()) {
        SDL_Log("スケルトン %s は有効な JSON ではありません", fileName.c_str());

        return false;
    }

    if (doc["version"].GetInt() != 1) {
        SDL_Log("スケルトン %s のバージョンが非対応です", fileName.c_str());

        return false;
    }

    const rapidjson::Value& bonecount = doc["bonecount"];

    if (!bonecount.IsUint()) {
        SDL_Log("スケルトン %s に bonecount がありません", fileName.c_str());

        return false;
    }

    size_t count = bonecount.GetUint();

    if (count > MAX_SKELETON_BONES) {
        SDL_Log("スケルトン %s のボーン数 %zu が上限 %zu を超えています", fileName.c_str(), count, MAX_SKELETON_BONES);

        return false;
    }

    mBones.reserve(count);

    const rapidjson::Value& bones = doc["bones"];

    if (!bones.IsArray()) {
        SDL_Log("スケルトン %s に bones 配列がありません", fileName.c_str());

        return false;
    }

    if (bones.Size() != count) {
        SDL_Log("スケルトン %s の bonecount %zu と bones 配列の要素数 %u が一致しません", fileName.c_str(), count, bones.Size());

        return false;
    }

    Bone temp;

    for (rapidjson::SizeType i = 0; i < count; i++) {
        if (!bones[i].IsObject()) {
            SDL_Log("スケルトン %s のボーン %u がオブジェクトではありません", fileName.c_str(), i);

            return false;
        }

        const rapidjson::Value& name = bones[i]["name"];
        temp.mName = name.GetString();

        const rapidjson::Value& parent = bones[i]["parent"];
        temp.mParent = parent.GetInt();

        const rapidjson::Value& bindpose = bones[i]["bindpose"];

        if (!bindpose.IsObject()) {
            SDL_Log("スケルトン %s のボーン %u に bindpose がありません", fileName.c_str(), i);

            return false;
        }

        const rapidjson::Value& rot = bindpose["rot"];
        const rapidjson::Value& trans = bindpose["trans"];

        if (!rot.IsArray() || !trans.IsArray()) {
            SDL_Log("スケルトン %s のボーン %u の rot / trans が配列ではありません", fileName.c_str(), i);

            return false;
        }

        temp.mLocalBindPose.mRotation.x = static_cast<float>(rot[0].GetDouble());
        temp.mLocalBindPose.mRotation.y = static_cast<float>(rot[1].GetDouble());
        temp.mLocalBindPose.mRotation.z = static_cast<float>(rot[2].GetDouble());
        temp.mLocalBindPose.mRotation.w = static_cast<float>(rot[3].GetDouble());

        temp.mLocalBindPose.mTranslation.x = static_cast<float>(trans[0].GetDouble());
        temp.mLocalBindPose.mTranslation.y = static_cast<float>(trans[1].GetDouble());
        temp.mLocalBindPose.mTranslation.z = static_cast<float>(trans[2].GetDouble());

        mBones.emplace_back(temp);
    }

    ComputeGlobalInvBindPose();

    return true;
}

void Skeleton::ComputeGlobalInvBindPose() {
    mGlobalInvBindPoses.resize(GetNumBones());

    mGlobalInvBindPoses[0] = mBones[0].mLocalBindPose.ToMatrix();

    for (size_t i = 1; i < mGlobalInvBindPoses.size(); i++) {
        auto localMat = mBones[i].mLocalBindPose.ToMatrix();
        mGlobalInvBindPoses[i] = localMat * mGlobalInvBindPoses[mBones[i].mParent];
    }

    for (auto& mGlobalInvBindPose : mGlobalInvBindPoses) {
        mGlobalInvBindPose.Invert();
    }
}
