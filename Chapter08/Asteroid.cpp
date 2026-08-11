#include "Asteroid.h"

#include "CircleComponent.h"
#include "Game.h"
#include "Math.h"
#include "MoveComponent.h"
#include "Random.h"
#include "SpriteComponent.h"

Asteroid::Asteroid(Game* game)
    : Actor(game) {
    const auto randPos = Random::GetVector(Vector2(-512.0f, -384.0f), Vector2(512.0f, 384.0f));
    SetPosition(randPos);
    SetRotation(Random::GetFloatRange(0.0f, Math::TwoPi));

    const auto sc = new SpriteComponent(this);
    sc->SetTexture(game->GetTexture("Assets/Asteroid.png"));

    const auto mc = new MoveComponent(this);
    mc->SetForwardSpeed(150.0f);

    mCircle = new CircleComponent(this);
    mCircle->SetRadius(40.0f);

    game->AddAsteroid(this);
}

Asteroid::~Asteroid() {
    GetGame()->RemoveAsteroid(this);
}