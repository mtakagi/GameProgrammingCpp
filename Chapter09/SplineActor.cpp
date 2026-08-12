#include "Game.h"
#include "SplineActor.h"
#include "SplineCamera.h"

SplineActor::SplineActor(Game *game)
    : Actor(game) {
    mCameraComp = new SplineCamera(this);

    Spline path;
    path.mControlPoints.emplace_back(Vector3::Zero);

    for (auto i = 0; i < 5; i++) {
        if (i % 2 == 0) {
            path.mControlPoints.emplace_back(300.0f * static_cast<float>(i + 1), 300.0f, 300.0f);
        } else {
            path.mControlPoints.emplace_back(300.0f * static_cast<float>(i + 1), 0.0f, 0.0f);
        }
    }

    mCameraComp->SetSpline(path);
    mCameraComp->SetPaused(false);
}

void SplineActor::ActorInput(const uint8_t *keyState) {
}

void SplineActor::RestartSpline() const {
    mCameraComp->Restart();
}
