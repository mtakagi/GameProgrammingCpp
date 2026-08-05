#include "Bullet.h"
#include "CircleComponent.h"
#include "Enemy.h"
#include "Game.h"
#include "MoveComponent.h"
#include "SpriteComponent.h"

Bullet::Bullet(Game* game)
    : Actor(game) {
    const auto sc = new SpriteComponent(this);
    sc->SetTexture(game->GetTexture("Assets/Projectile.png"));

    const auto mc = new MoveComponent(this);
    mc->SetForwardSpeed(400.0f);

    mCircle = new CircleComponent(this);
    mCircle->SetRadius(5.0f);

    mLiveTime = 1.0f;
}

void Bullet::UpdateActor(const float deltaTime) {
    Actor::UpdateActor(deltaTime);

    for (auto& e : GetGame()->GetEnemies()) {
        if (Intersect(*mCircle, *e->GetCircle())) {
            e->SetState(EDead);
            SetState(EDead);
            break;
        }
    }

    mLiveTime -= deltaTime;
    if (mLiveTime <= 0.0f) {
        SetState(EDead);
    }
}
