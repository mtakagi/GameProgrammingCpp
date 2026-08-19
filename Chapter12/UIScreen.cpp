#include "Game.h"
#include "Font.h"
#include "Renderer.h"
#include "Shader.h"
#include "Texture.h"
#include "UIScreen.h"

UIScreen::UIScreen(Game *game)
    : mGame(game)
    , mTitle(nullptr)
    , mBackground(nullptr)
    , mTitlePos(0.0f, 300.0f)
    , mNextButtonPos(0.0f, 200.0f)
    , mBGPos(0.0f, 250.0f)
    , mState(EActive) {
    mGame->PushUI(this);
    mFont = mGame->GetFont("Assets/Carlito-Regular.ttf");
    mButtonOn = mGame->GetRenderer()->GetTexture("Assets/ButtonYellow.png");
    mButtonOff = mGame->GetRenderer()->GetTexture("Assets/ButtonBlue.png");
}

UIScreen::~UIScreen() {
    if (mTitle) {
        mTitle->Unload();
        delete mTitle;
    }

    for (const auto& button : mButtons) {
        delete button;
    }
    mButtons.clear();
}

void UIScreen::Update(const float deltaTime) {
}

void UIScreen::Draw(Shader *shader) {
    if (mBackground) {
        DrawTexture(shader, mBackground, mBGPos);
    }

    if (mTitle) {
        DrawTexture(shader, mTitle, mTitlePos);
    }

    for (const auto& button : mButtons) {
        const auto tex = button->GetHighlighted() ? mButtonOn : mButtonOff;
        DrawTexture(shader, tex, button->GetPosition());
        DrawTexture(shader, button->GetNameTex(), button->GetPosition());
    }
}

void UIScreen::ProcessInput(const uint8_t *keyState) {
    if (!mButtons.empty()) {
        int x, y;
        SDL_GetMouseState(&x, &y);
        Vector2 mousePos(x, y);

        mousePos.x -= mGame->GetRenderer()->GetScreenWidth() * 0.5f;
        mousePos.y = mGame->GetRenderer()->GetScreenHeight() * 0.5f - mousePos.y;

        for (const auto& button : mButtons) {
            if (button->ContainsPoint(mousePos)) {
                button->SetHighlighted(true);
            } else {
                button->SetHighlighted(false);
            }
        }
    }
}

void UIScreen::HandleKeyPress(int key) {
    switch (key) {
        case SDL_BUTTON_LEFT:
            if (!mButtons.empty()) {
                for (const auto& button : mButtons) {
                    if (button->GetHighlighted()) {
                        button->OnClick();
                        break;
                    }
                }
            }
            break;
        default:
            break;
    }
}

void UIScreen::Close() {
    mState = EClosing;
}

void UIScreen::SetTitle(const std::string &text, const Vector3 &color, const int pointSize) {
    if (mTitle) {
        mTitle->Unload();
        delete mTitle;
        mTitle = nullptr;
    }
    mTitle = mFont->RenderText(text, color, pointSize);
}

void UIScreen::AddButton(const std::string &name, const std::function<void()>& onClick) {
    const Vector2 dims(mButtonOn->GetWidth(), mButtonOn->GetHeight());
    const auto button = new Button(name, mFont, onClick, mNextButtonPos, dims);

    mButtons.push_back(button);
    mNextButtonPos.y -= mButtonOff->GetHeight() + 20.0f;
}

void UIScreen::DrawTexture(const Shader *shader, const Texture *texture, const Vector2 &offset, const float scale) {
    const auto scaleMat = Matrix4::CreateScale(texture->GetWidth() * scale, texture->GetHeight() * scale, 1.0f);
    const auto transMat = Matrix4::CreateTranslation(Vector3(offset.x, offset.y, 0.0f));
    const auto world = scaleMat * transMat;

    shader->SetMatrixUniform("uWorldTransform", world);
    texture->SetActive();
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}

void UIScreen::SetRelativeMouseMode(const bool relative) {
    if (relative) {
        SDL_SetRelativeMouseMode(SDL_TRUE);
        SDL_GetRelativeMouseState(nullptr, nullptr);
    } else {
        SDL_SetRelativeMouseMode(SDL_FALSE);
    }
}

Button::Button(const std::string &name, Font *font, const std::function<void()>& onClick, const Vector2 &pos, const Vector2 &dims)
    : mOnClick(onClick)
    , mNameTex(nullptr)
    , mFont(font)
    , mPosition(pos)
    , mDimensions(dims)
    , mHighlighted(false) {
    SetName(name);
}

Button::~Button() {
    if (mNameTex) {
        mNameTex->Unload();
        delete mNameTex;
    }
}

void Button::SetName(const std::string &name) {
    mName = name;

    if (mNameTex) {
        mNameTex->Unload();
        delete mNameTex;
        mNameTex = nullptr;
    }

    mNameTex = mFont->RenderText(name);
}

bool Button::ContainsPoint(const Vector2 &pt) const {
    const auto no = pt.x < (mPosition.x - mDimensions.x / 2.0f) ||
        pt.x > (mPosition.x + mDimensions.x / 2.0f) ||
        pt.y < (mPosition.y - mDimensions.y / 2.0f) ||
        pt.y > (mPosition.y + mDimensions.y / 2.0f);

    return !no;
}

void Button::OnClick() const {
    if (mOnClick) {
        mOnClick();
    }
}
