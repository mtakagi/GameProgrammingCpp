#include "Actor.h"
#include "SplineCamera.h"

Vector3 Spline::Compute(const size_t startIdx, const float t) const {
    if (startIdx >= mControlPoints.size()) {
        return mControlPoints.back();
    }
    if (startIdx == 0) {
        return mControlPoints[startIdx];
    }
    if (startIdx + 2 >= mControlPoints.size()) {
        return mControlPoints[startIdx];
    }

    const auto p0 = mControlPoints[startIdx - 1];
    const auto p1 = mControlPoints[startIdx];
    const auto p2 = mControlPoints[startIdx + 1];
    const auto p3 = mControlPoints[startIdx + 2];
    const auto position = 0.5f * ((2.0f * p1) + (-1.0f * p0 + p2) * t + (2.0f * p0 - 5.0f * p1 + 4.0f * p2 - p3) * t * t + (-1.0f * p0 + 3.0f * p1 - 3.0f * p2 + p3) * t * t *t);

    return position;
}

SplineCamera::SplineCamera(Actor *owner)
    : CameraComponent(owner)
    , mIndex(1)
    , mT(0.0f)
    , mSpeed(0.5f)
    , mPaused(true) {
}

void SplineCamera::Update(const float deltaTime) {
    CameraComponent::Update(deltaTime);

    if (!mPaused) {
        mT += mSpeed * deltaTime;

        if (mT >= 1.0f) {
            if (mIndex < mPath.GetNumPoints() - 3) {
                mIndex++;
                mT = mT - 1.0f;
            } else {
                mPaused = true;
            }
        }
    }

    const auto cameraPos = mPath.Compute(mIndex, mT);
    const auto target = mPath.Compute(mIndex, mT + 0.01f);
    const auto up = Vector3::UnitZ;
    const auto view = Matrix4::CreateLookAt(cameraPos, target, up);

    SetViewMatrix(view);
}

void SplineCamera::Restart() {
    mIndex = 1;
    mT = 0.0f;
    mPaused = false;
}
