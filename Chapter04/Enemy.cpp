#include <algorithm>
#include <ranges>
#include "CircleComponent.h"
#include "Enemy.h"
#include "Game.h"
#include "Grid.h"
#include "Math.h"
#include "NavComponent.h"
#include "SpriteComponent.h"
#include "Tile.h"

Enemy::Enemy(Game *game)
    : Actor(game){
    game->GetEnemies().emplace_back(this);

    const auto sc = new SpriteComponent(this);
    sc->SetTexture(game->GetTexture("Assets/Airplane.png"));

    SetPosition(GetGame()->GetGrid()->GetStartTile()->GetPosition());

    auto nc = new NavComponent(this);
    nc->SetForwardSpeed(150.0f);
    nc->StartPath(GetGame()->GetGrid()->GetStartTile());

    mCircle = new CircleComponent(this);
    mCircle->SetRadius(25.0f);
}

Enemy::~Enemy() {
   auto iter = std::ranges::find(GetGame()->GetEnemies(), this);
    GetGame()->GetEnemies().erase(iter);
}

void Enemy::UpdateActor(const float deltaTime) {
    Actor::UpdateActor(deltaTime);

    auto diff = GetPosition() - GetGame()->GetGrid()->GetEndTile()->GetPosition();

    if (Math::NearZero(diff.Length(), 10.0f)) {
        SetState(EDead);
    }
}
