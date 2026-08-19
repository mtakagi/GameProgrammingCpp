#pragma once

#include <string>
#include <vector>
#include "Math.h"

class BoneTransform;
class Skeleton;

class Animation {
public:
    bool Load(const std::string& fileName);

    [[nodiscard]]
    size_t GetNumBones() const { return mNumBones; }
    [[nodiscard]]
    size_t GetNumFrames() const { return mNumFrames; }
    [[nodiscard]]
    float GetDuration() const { return mDuration; }
    [[nodiscard]]
    float GetFrameDuration() const { return mFrameDuration; }

    void GetGlobalPoseAtTime(std::vector<Matrix4>& outPoses, const Skeleton* inSkeleton, float inTime) const;

private:
    size_t mNumBones = 0;
    size_t mNumFrames = 0;
    float mDuration = 0;
    float mFrameDuration = 0;

    std::vector<std::vector<BoneTransform>> mTracks;
};

