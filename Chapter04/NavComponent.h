#pragma once

#include "MoveComponent.h"

class Actor;
class Tile;
class Vector2;

class NavComponent : public MoveComponent {
public:
    explicit NavComponent(Actor* owner, int updateOrder = 10);

    void Update(float deltaTime) override;
    void StartPath(const Tile* start);
    void TurnTo(const Vector2& pos) const;

private:
    const Tile* mNextNode;
};
