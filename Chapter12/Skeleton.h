#pragma once

#include <string>
#include <vector>
#include "BoneTransform.h"

class Skeleton {
public:
    struct Bone {
        BoneTransform mLocalBindPose;
        std::string mName;
        int mParent;
    };

    bool Load(const std::string& fileName);

    [[nodiscard]]
    size_t GetNumBones() const { return mBones.size(); }
    [[nodiscard]]
    const Bone& GetBone(const size_t idx) const { return mBones[idx]; }
    [[nodiscard]]
    const std::vector<Bone>& GetBones() const { return mBones; }
    [[nodiscard]]
    const std::vector<Matrix4>& GetGlobalInvBindPoses() const { return mGlobalInvBindPoses; }

protected:
    void ComputeGlobalInvBindPose();

private:
    std::vector<Bone> mBones;
    std::vector<Matrix4> mGlobalInvBindPoses;
};
