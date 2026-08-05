#include <algorithm>
#include "Enemy.h"
#include "Grid.h"
#include "Math.h"
#include "Tile.h"
#include "Tower.h"

Grid::Grid(Game *game)
    : Actor(game)
    , mSelectedTile(nullptr)
    , mNextEnemy(0.0f){
    mTiles.resize(NumRows);

    for (auto& mTile : mTiles) {
        mTile.resize(NumCols);
    }

    for (auto i = 0; i < NumRows; ++i) {
        for (auto j = 0; j < NumCols; ++j) {
            mTiles[i][j] = new Tile(game);
            mTiles[i][j]->SetPosition(Vector2(TileSize / 2.0f + j * TileSize, StartY + i * TileSize));
        }
    }

    GetStartTile()->SetTileState(Tile::EStart);
    GetEndTile()->SetTileState(Tile::EBase);

    for (auto i = 0; i < NumRows; ++i) {
        for (auto j = 0; j < NumCols; ++j) {
            if (i > 0) {
                mTiles[i][j]->mAdjacent.push_back(mTiles[i - 1][j]);
            }
            if (i < NumRows - 1) {
                mTiles[i][j]->mAdjacent.push_back(mTiles[i + 1][j]);
            }
            if (j > 0) {
                mTiles[i][j]->mAdjacent.push_back(mTiles[i][j - 1]);
            }
            if (j < NumCols - 1) {
                mTiles[i][j]->mAdjacent.push_back(mTiles[i][j + 1]);
            }
        }
    }

    FindPath(GetEndTile(), GetStartTile());
    UpdatePathTiles(GetStartTile());

    mNextEnemy = EnemyTime;
}

void Grid::SelectTile(const size_t row, const size_t col) {
    const auto tstate = mTiles[row][col]->GetTileState();

    if (tstate != Tile::EStart && tstate != Tile::EBase) {
        if (mSelectedTile) {
            mSelectedTile->ToggleSelect();
        }

        mSelectedTile = mTiles[row][col];
        mSelectedTile->ToggleSelect();
    }
}

void Grid::ProcessClick(int x, int y) {
    y -= static_cast<int>(StartY - TileSize / 2);

    if (y >= 0) {
        x /= static_cast<int>(TileSize);
        y /= static_cast<int>(TileSize);

        if (x >= 0 && x < static_cast<int>(NumCols) && y >= 0 && y < static_cast<int>(NumRows)) {
            SelectTile(y, x);
        }
    }
}

bool Grid::FindPath(Tile *start, const Tile *goal) {
    for (auto i = 0; i < NumRows; ++i) {
        for (auto j = 0; j < NumCols; ++j) {
            mTiles[i][j]->g = 0.0f;
            mTiles[i][j]->mInOpenSet = false;
            mTiles[i][j]->mInClosedSet = false;
        }
    }

    std::vector<Tile*> openSet;
    Tile* current = start;
    current->mInClosedSet = true;

    do {
       for (auto& neighbor : current->mAdjacent) {
           if (neighbor->mBlocked) {
               continue;
           }

           if (!neighbor->mInClosedSet) {
               if (!neighbor->mInOpenSet) {
                   neighbor->mParent = current;
                   neighbor->h = (neighbor->GetPosition() - goal->GetPosition()).Length();
                   neighbor->g = current->g + TileSize;
                   neighbor->f = neighbor->g + neighbor->h;
                   openSet.emplace_back(neighbor);
                   neighbor->mInOpenSet = true;
               } else {
                   auto newG = current->g + TileSize;

                   if (newG < neighbor->g) {
                       neighbor->mParent = current;
                       neighbor->g = newG;
                       neighbor->f = neighbor->g + neighbor->h;
                   }
               }
           }
       }

        if (openSet.empty()) {
            break;
        }

        auto iter = std::ranges::min_element(openSet,
                                             [](const Tile* a, const Tile* b) {
                                                 return a->f < b->f;
                                             });
        current = *iter;
        openSet.erase(iter);
        current->mInOpenSet = false;
        current->mInClosedSet = true;
    } while (current != goal);

    return current == goal;
}

void Grid::UpdatePathTiles(const Tile* start) const {
    for (auto i = 0; i < NumRows; ++i) {
        for (auto j = 0; j < NumCols; ++j) {
            if (!(i == 3 && j == 0) && !(i == 3 && j == 15)) {
                mTiles[i][j]->SetTileState(Tile::EDefault);
            }
        }
    }

    auto t = start->mParent;

    while (t != GetEndTile()) {
        t->SetTileState(Tile::EPath);
        t = t->mParent;
    }
}

void Grid::BuildTower() {
    if (mSelectedTile && !mSelectedTile->mBlocked) {
        mSelectedTile->mBlocked = true;

        if (FindPath(GetEndTile(), GetStartTile())) {
            auto t = new Tower(GetGame());
            t->SetPosition(mSelectedTile->GetPosition());
        } else {
            mSelectedTile->mBlocked = false;
            FindPath(GetEndTile(), GetStartTile());
        }

        UpdatePathTiles(GetStartTile());
    }
}

Tile *Grid::GetStartTile() const {
    return mTiles[3][0];
}

Tile* Grid::GetEndTile() const {
    return mTiles[3][15];
}

void Grid::UpdateActor(const float deltaTime) {
    Actor::UpdateActor(deltaTime);

    mNextEnemy -= deltaTime;

    if (mNextEnemy < 0.0f) {
        new Enemy(GetGame());
        mNextEnemy += EnemyTime;
    }
}
