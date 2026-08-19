#pragma once

#include "MatrixPalette.h"
#include "MeshComponent.h"

class Actor;
class Animation;
class Shader;
class Skeleton;

class SkeletalMeshComponent : public MeshComponent {
public:
    explicit SkeletalMeshComponent(Actor* owner);
    void Draw(Shader* shader) override;

    void Update(float deltaTime) override;

    void SetSkeleton(const Skeleton* sk) { mSkeleton = sk;}

    float PlayAnimation(const Animation* anim, float playRate = 1.0f);

private:
    void ComputeMatrixPalette();

    MatrixPalette mPalette;
    const Skeleton* mSkeleton;
    const Animation* mAnimation;
    float mAnimPlayRate;
    float mAnimTime;
};
