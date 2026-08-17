#include "DialogBox.h"
#include "Game.h"
#include "PauseMenu.h"

PauseMenu::PauseMenu(Game* game)
    : UIScreen(game) {
    mGame->SetState(Game::EPaused);
    SetRelativeMouseMode(false);
    SetTitle("PauseTitle");
    AddButton("ResumeButton", [this] {
        Close();
    });
    AddButton("QuitButton", [this] {
        new DialogBox(mGame, "QuitText", [this] {
            mGame->SetState(Game::EQuit);
        });
    });
}

PauseMenu::~PauseMenu() {
    SetRelativeMouseMode(true);
    mGame->SetState(Game::EGamePlay);
}

void PauseMenu::HandleKeyPress(const int key) {
    UIScreen::HandleKeyPress(key);

    if (key == SDLK_ESCAPE) {
        Close();
    }
}
