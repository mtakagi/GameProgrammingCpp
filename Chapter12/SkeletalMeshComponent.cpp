#include "Actor.h"
#include "Animation.h"
#include "Mesh.h"
#include "SkeletalMeshComponent.h"
#include "Skeleton.h"
#include "Shader.h"
#include "Texture.h"
#include "VertexArray.h"

SkeletalMeshComponent::SkeletalMeshComponent(Actor *owner)
    : MeshComponent(owner, true)
    , mSkeleton(nullptr)
    , mAnimation(nullptr)
    , mAnimPlayRate(0.0f)
    , mAnimTime(0.0f) {
}

void SkeletalMeshComponent::Draw(Shader *shader) {
    if (mMesh) {
        shader->SetMatrixUniform("uWorldTransform", mOwner->GetWorldTransform());
        shader->SetMatrixUniforms("uMatrixPalette", &mPalette.mEntry[0], MAX_SKELETON_BONES);
        shader->SetFloatUniform("uSpecPower", mMesh->GetSpecPower());

        auto t = mMesh->GetTexture(mTextureIndex);

        if (t) {
            t->SetActive();
        }

        const auto va = mMesh->GetVertexArray();
        va->SetActive();

        glDrawElements(GL_TRIANGLES, va->GetNumIndices(), GL_UNSIGNED_INT, nullptr);
    }
}

void SkeletalMeshComponent::Update(float deltaTime) {
    if (mAnimation && mSkeleton) {
        mAnimTime += deltaTime * mAnimPlayRate;

        while (mAnimTime >= mAnimation->GetDuration()) {
            mAnimTime -= mAnimation->GetDuration();
        }

        ComputeMatrixPalette();
    }
}

float SkeletalMeshComponent::PlayAnimation(const Animation *anim, float playRate) {
    mAnimation = anim;
    mAnimTime = 0.0f;
    mAnimPlayRate = playRate;

    if (!mAnimation) {
        return 0.0f;
    }

    ComputeMatrixPalette();

    return mAnimation->GetDuration();
}

void SkeletalMeshComponent::ComputeMatrixPalette() {
    const auto& globalInvBindPases = mSkeleton->GetGlobalInvBindPoses();
    std::vector<Matrix4> currentPoses;

    mAnimation->GetGlobalPoseAtTime(currentPoses, mSkeleton, mAnimTime);

    for (size_t i = 0; i < mSkeleton->GetNumBones(); i++) {
        mPalette.mEntry[i] = globalInvBindPases[i] * currentPoses[i];
    }
}

