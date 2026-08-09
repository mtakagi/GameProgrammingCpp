#include "Game.h"
#include "MeshComponent.h"
#include "PlaneActor.h"
#include "Renderer.h"

PlaneActor::PlaneActor(Game* game)
    : Actor(game) {
    SetScale(10.0f);
    const auto mc = new MeshComponent(this);
    mc->SetMesh(GetGame()->GetRenderer()->GetMesh("Assets/Plane.gpmesh"));
}
