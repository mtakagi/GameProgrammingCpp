#include "Bullet.h"
#include "Enemy.h"
#include "Game.h"
#include "MoveComponent.h"
#include "SpriteComponent.h"
#include "Tower.h"

Tower::Tower(Game *game)
    : Actor(game) {
    const auto sc = new SpriteComponent(this);
    sc->SetTexture(game->GetTexture("Assets/Tower.png"));

    mMove = new MoveComponent(this);
    // mMove->SetAngularSpeed(Math::Pi);

    mNextAttack = AttackTime;
}

void Tower::UpdateActor(const float deltaTime) {
    Actor::UpdateActor(deltaTime);

    mNextAttack -= deltaTime;

    if (mNextAttack <= 0.0f) {
        auto e = GetGame()->GetNearestEnemy(GetPosition());

        if (e != nullptr) {
            const auto dir = e->GetPosition() - GetPosition();
            const auto dist = dir.Length();

            if (dist < AttackRange) {
                SetRotation(Math::Atan2(-dir.y, dir.x));

                const auto b = new Bullet(GetGame());
                b->SetPosition(GetPosition());
                b->SetRotation(GetRotation());
            }
        }

        mNextAttack += AttackTime;
    }
}