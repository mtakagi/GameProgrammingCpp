#pragma once

#include "UIScreen.h"

class Game;

class PauseMenu : public UIScreen {
public:
    explicit PauseMenu(Game* game);
    ~PauseMenu() override;

    void HandleKeyPress(int key) override;
};
