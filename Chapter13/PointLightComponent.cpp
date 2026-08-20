#include "Actor.h"
#include "Game.h"
#include "PointLightComponent.h"
#include "Renderer.h"
#include "Shader.h"
#include "VertexArray.h"
#include "Math.h"

PointLightComponent::PointLightComponent(Actor *owner)
    : Component(owner)
    , mInnerRadius(0.0f)
    , mOuterRadius(0.0f){
    owner->GetGame()->GetRenderer()->AddPointLight(this);
}

PointLightComponent::~PointLightComponent() {
    mOwner->GetGame()->GetRenderer()->RemovePointLight(this);
}

void PointLightComponent::Draw(const Shader *shader, const Mesh *mesh) {
    const auto scale = Matrix4::CreateScale(mOwner->GetScale() * mOuterRadius / mesh->GetRadius());
    const auto trans = Matrix4::CreateTranslation(mOwner->GetPosition());
    const auto worldTransform = scale * trans;

    shader->SetMatrixUniform("uWorldTransform", worldTransform);
    shader->SetVectorUniform("uPointLight.mWorldPos", mOwner->GetPosition());
    shader->SetVectorUniform("uPointLight.mDiffuseColor", mDiffuseColor);
    shader->SetFloatUniform("uPointLight.mInnerRadius", mInnerRadius);
    shader->SetFloatUniform("uPointLight.mOuterRadius", mOuterRadius);

    glDrawElements(GL_TRIANGLES, mesh->GetVertexArray()->GetNumIndices(), GL_UNSIGNED_INT, nullptr);
}
