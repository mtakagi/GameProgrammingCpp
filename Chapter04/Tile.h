#pragma once

#include "Actor.h"

class Game;
class SpriteComponent;

class Tile : public Actor {
public:
    friend class Grid;

    enum  TileState {
        EDefault,
        EPath,
        EStart,
        EBase,
    };

    explicit Tile(Game* game);

    void SetTileState(TileState state);
    [[nodiscard]]
    TileState GetTileState() const { return mTileState; }
    void ToggleSelect();
    [[nodiscard]]
    const Tile* GetParent() const { return mParent; }

private:
    std::vector<Tile*> mAdjacent;
    Tile* mParent;
    float f;
    float g;
    float h;
    bool mInOpenSet;
    bool mInClosedSet;
    bool mBlocked;

    void UpdateTexture() const;
    SpriteComponent* mSprite;
    TileState mTileState;
    bool mSelected;
};
