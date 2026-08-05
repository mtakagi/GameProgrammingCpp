#include <string>
#include "Game.h"
#include "SpriteComponent.h"
#include "Tile.h"

Tile::Tile(Game* game)
    : Actor(game)
    , mParent(nullptr)
    , f(0.0f)
    , g(0.0f)
    , h(0.0f)
    , mInOpenSet(false)
    , mInClosedSet(false)
    , mBlocked(false)
    , mSprite(nullptr)
    , mTileState(EDefault)
    , mSelected(false) {
    mSprite = new SpriteComponent(this);
    UpdateTexture();
}

void Tile::SetTileState(const TileState state) {
    mTileState = state;
    UpdateTexture();
}

void Tile::ToggleSelect() {
    mSelected = !mSelected;
    UpdateTexture();
}

void Tile::UpdateTexture() const {
    std::string text;
    switch (mTileState) {
        case EStart:
            text = "Assets/TileTan.png";
            break;
        case EBase:
            text = "Assets/TileGreen.png";
            break;
        case EPath:
            if (mSelected) {
                text = "Assets/TileGreySelected.png";
            } else {
                text = "Assets/TileGrey.png";
            }
            break;
        case EDefault:
            if (mSelected) {
                text = "Assets/TileBrownSelected.png";
            } else {
                text = "Assets/TileBrown.png";
            }
            break;
    }

    mSprite->SetTexture(GetGame()->GetTexture(text));
}
