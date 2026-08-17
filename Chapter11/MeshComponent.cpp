#include "Actor.h"
#include "Game.h"
#include "Shader.h"
#include "Renderer.h"
#include "Mesh.h"
#include "MeshComponent.h"
#include "Texture.h"
#include "VertexArray.h"

MeshComponent::MeshComponent(Actor* owner)
    : Component(owner)
    , mMesh(nullptr)
    , mTextureIndex(0)
    , mVisible(true) {
    mOwner->GetGame()->GetRenderer()->AddMeshComp(this);
}

MeshComponent::~MeshComponent() {
    mOwner->GetGame()->GetRenderer()->RemoveMeshComp(this);
}

void MeshComponent::Draw(Shader *shader) {
    if (mMesh) {
        shader->SetMatrixUniform("uWorldTransform", mOwner->GetWorldTransform());
        shader->SetFloatUniform("uSpecPower", mMesh->GetSpecPower());
        const auto t = mMesh->GetTexture(mTextureIndex);

        if (t) {
            t->SetActive();
        }

        const auto va = mMesh->GetVertexArray();
        va->SetActive();

        glDrawElements(GL_TRIANGLES, va->GetNumIndices(), GL_UNSIGNED_INT, nullptr);
    }
}
