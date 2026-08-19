#include "BoneTransform.h"

Matrix4 BoneTransform::ToMatrix() const {
    const auto rot = Matrix4::CreateFromQuaternion(mRotation);
    const auto trans = Matrix4::CreateTranslation(mTranslation);

    return rot * trans;
}

BoneTransform BoneTransform::Interpolate(const BoneTransform &a, const BoneTransform &b, const float f) {
    BoneTransform retVal;

    retVal.mRotation = Quaternion::Slerp(a.mRotation, b.mRotation, f);
    retVal.mTranslation = Vector3::Lerp(a.mTranslation, b.mTranslation, f);

    return retVal;
}
