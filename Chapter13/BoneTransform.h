#pragma once

#include "Math.h"

struct BoneTransform {
    Quaternion mRotation;
    Vector3 mTranslation;

    [[nodiscard]]
    Matrix4 ToMatrix() const;

    static BoneTransform Interpolate(const BoneTransform& a, const BoneTransform& b, float f);
};

