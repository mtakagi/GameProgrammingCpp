#pragma once

#include "UIScreen.h"

class Game;

class DialogBox : public UIScreen {
public:
    explicit DialogBox(Game* game, const std::string& text, const std::function<void()>& onOK);
    ~DialogBox() override;
};
