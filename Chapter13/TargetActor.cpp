#include "BoxComponent.h"
#include "Game.h"
#include "Mesh.h"
#include "MeshComponent.h"
#include "Renderer.h"
#include "TargetActor.h"
#include "TargetComponent.h"

TargetActor::TargetActor(Game *game)
    : Actor(game) {
    SetRotation(Quaternion(Vector3::UnitZ, Math::Pi));
    const auto mc = new MeshComponent(this);
    const auto mesh = GetGame()->GetRenderer()->GetMesh("Assets/Target.gpmesh");
    mc->SetMesh(mesh);

    const auto bc = new BoxComponent(this);
    bc->SetObjectBox(mesh->GetBox());

    new TargetComponent(this);
}
