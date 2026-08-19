#include <fstream>
#include <SDL.h>
#include <sstream>
#include <rapidjson/document.h>
#include "Animation.h"
#include "BoneTransform.h"
#include "Skeleton.h"

bool Animation::Load(const std::string &fileName) {
    std::ifstream file(fileName);

    if (!file.is_open()) {
        SDL_Log("アニメーションファイル %s を開けません", fileName.c_str());

        return false;
    }

    std::stringstream fileStream;
    fileStream << file.rdbuf();
    std::string contents = fileStream.str();
    rapidjson::StringStream jsonStr(contents.c_str());
    rapidjson::Document doc;

    doc.ParseStream(jsonStr);

    if (!doc.IsObject()) {
        SDL_Log("アニメーション %s は有効な JSON ではありません", fileName.c_str());

        return false;
    }

    if (doc["version"].GetInt() != 1) {
        SDL_Log("アニメーション %s のバージョンが非対応です", fileName.c_str());

        return false;
    }

    const rapidjson::Value& sequence = doc["sequence"];

    if (!sequence.IsObject()) {
        SDL_Log("アニメーション %s に sequence がありません", fileName.c_str());

        return false;
    }

    const rapidjson::Value& frames = sequence["frames"];
    const rapidjson::Value& length = sequence["length"];
    const rapidjson::Value& bonecount = sequence["bonecount"];

    if (!frames.IsUint() || !length.IsDouble() || !bonecount.IsUint()) {
        SDL_Log("アニメーション %s の frames / length / bonecount が不正です", fileName.c_str());

        return false;
    }

    mNumFrames = frames.GetUint();
    mDuration = static_cast<float>(length.GetDouble());
    mNumBones = bonecount.GetUint();
    mFrameDuration = mDuration / static_cast<float>(mNumFrames - 1);

    mTracks.resize(mNumBones);

    const rapidjson::Value& tracks = sequence["tracks"];

    if (!tracks.IsArray()) {
        SDL_Log("アニメーション %s に tracks 配列がありません", fileName.c_str());

        return false;
    }

    for (rapidjson::SizeType i = 0; i < tracks.Size(); i++) {
        if (!tracks[i].IsObject()) {
            SDL_Log("アニメーション %s のトラック %u がオブジェクトではありません", fileName.c_str(), i);

            return false;
        }

        size_t boneIndex = tracks[i]["bone"].GetUint();
        const rapidjson::Value& transforms = tracks[i]["transforms"];

        if (!transforms.IsArray()) {
            SDL_Log("アニメーション %s のトラック %u に transforms 配列がありません", fileName.c_str(), i);

            return false;
        }

        BoneTransform temp;

        if (transforms.Size() < mNumFrames) {
            SDL_Log("アニメーション %s のトラック %u のフレーム数が %u で、必要な %zu に足りません", fileName.c_str(), i, transforms.Size(), mNumFrames);

            return false;
        }

        for (rapidjson::SizeType j = 0; j < transforms.Size(); j++) {
            const rapidjson::Value& rot = transforms[j]["rot"];
            const rapidjson::Value& trans = transforms[j]["trans"];

            if (!rot.IsArray() || !trans.IsArray()) {
                SDL_Log("アニメーション %s のトラック %u のフレーム %u の rot / trans が配列ではありません", fileName.c_str(), i, j);

                return false;
            }

            temp.mRotation.x = static_cast<float>(rot[0].GetDouble());
            temp.mRotation.y = static_cast<float>(rot[1].GetDouble());
            temp.mRotation.z = static_cast<float>(rot[2].GetDouble());
            temp.mRotation.w = static_cast<float>(rot[3].GetDouble());

            temp.mTranslation.x = static_cast<float>(trans[0].GetDouble());
            temp.mTranslation.y = static_cast<float>(trans[1].GetDouble());
            temp.mTranslation.z = static_cast<float>(trans[2].GetDouble());

            mTracks[boneIndex].emplace_back(temp);
        }
    }

    return true;
}

void Animation::GetGlobalPoseAtTime(std::vector<Matrix4> &outPoses, const Skeleton *inSkeleton, float inTime) const {
    if (outPoses.size() != mNumBones) {
        outPoses.resize(mNumBones);
    }

    const auto frame = static_cast<size_t>(inTime / mFrameDuration);
    const auto nextFrame = frame + 1;
    const auto pct = inTime / mFrameDuration - static_cast<float>(frame);

    if (!mTracks[0].empty()) {
        const auto interp = BoneTransform::Interpolate(mTracks[0][frame], mTracks[0][nextFrame], pct);
        outPoses[0] = interp.ToMatrix();
    } else {
        outPoses[0] = Matrix4::Identity;
    }

    const auto& bones = inSkeleton->GetBones();

    for (size_t bone = 1; bone < mNumBones; bone++) {
        Matrix4 localMat;

        if (!mTracks[bone].empty()) {
            const auto interp = BoneTransform::Interpolate(mTracks[bone][frame], mTracks[bone][nextFrame], pct);
            localMat = interp.ToMatrix();
        }

        outPoses[bone] = localMat * outPoses[bones[bone].mParent];
    }
}
