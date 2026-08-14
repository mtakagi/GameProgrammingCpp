#include "BoxComponent.h"
#include "Game.h"
#include "MeshComponent.h"
#include "PlaneActor.h"
#include "Renderer.h"

PlaneActor::PlaneActor(Game* game)
    : Actor(game) {
    SetScale(10.0f);
    const auto mc = new MeshComponent(this);
    const auto mesh = GetGame()->GetRenderer()->GetMesh("Assets/Plane.gpmesh");
    mc->SetMesh(mesh);

    mBox = new BoxComponent(this);
    mBox->SetObjectBox(mesh->GetBox());

    game->AddPlane(this);
}

PlaneActor::~PlaneActor() {
    GetGame()->RemovePlane(this);
}
