#pragma once

#include "Actor.h"

class Game;
class Tile;

class Grid : public Actor {
public:
    explicit Grid(Game* game);

    void ProcessClick(int x, int y);

    bool FindPath(Tile* start, const Tile* goal);

    void BuildTower();

    [[nodiscard]]
    Tile* GetStartTile() const;
    [[nodiscard]]
    Tile* GetEndTile() const;

    void UpdateActor(float deltaTime) override;

private:
    void SelectTile(size_t row, size_t col);
    void UpdatePathTiles(const Tile* start) const;

    Tile* mSelectedTile;

    std::vector<std::vector<Tile*>> mTiles;

    float mNextEnemy;

    const size_t NumRows = 7;
    const size_t NumCols = 16;
    const float StartY = 192.0f;
    const float TileSize = 64.0f;
    const float EnemyTime = 1.5f;
};
