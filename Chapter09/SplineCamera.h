#pragma once

#include <vector>
#include "CameraComponent.h"

struct Spline {
    std::vector<Vector3> mControlPoints;

    [[nodiscard]]
    Vector3 Compute(size_t startIdx, float t) const;

    [[nodiscard]]
    size_t GetNumPoints() const { return mControlPoints.size(); }
};

class Actor;

class SplineCamera : public CameraComponent {
public:
    explicit SplineCamera(Actor* owner);

    void Update(float deltaTime) override;
    void Restart();

    void SetSpeed(const float speed) { mSpeed = speed; }
    void SetSpline(const Spline& spline) { mPath = spline; }
    void SetPaused(const bool pause) { mPaused = pause; }

private:
    Spline mPath;
    size_t mIndex;

    float mT;
    float mSpeed;
    bool mPaused;
};
