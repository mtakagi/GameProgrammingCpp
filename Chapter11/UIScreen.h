#pragma once

#include <cstdint>
#include <functional>
#include <string>
#include <vector>
#include "Math.h"

class Font;
class Game;
class Shader;
class Texture;

class Button {
public:
    explicit Button(const std::string& name, Font* font, const std::function<void()>& onClick, const Vector2& pos, const Vector2& dims);
    ~Button();

    void SetName(const std::string& name);

    [[nodiscard]]
    Texture* GetNameTex() const { return mNameTex; }
    [[nodiscard]]
    const Vector2& GetPosition() const { return mPosition; }
    void SetHighlighted(const bool sel) { mHighlighted = sel; }
    [[nodiscard]]
    bool GetHighlighted() const { return mHighlighted; }

    [[nodiscard]]
    bool ContainsPoint(const Vector2& pt) const;
    void OnClick() const;

private:
    std::function<void()> mOnClick;
    std::string mName;
    Texture* mNameTex;
    Font* mFont;
    Vector2 mPosition;
    Vector2 mDimensions;
    bool mHighlighted;
};

class UIScreen {
public:
    explicit UIScreen(Game* game);
    virtual ~UIScreen();

    virtual void Update(float deltaTime);
    virtual void Draw(Shader* shader);
    virtual void ProcessInput(const uint8_t* keyState);
    virtual void HandleKeyPress(int key);

    enum UIState {
        EActive,
        EClosing,
    };

    void Close();
    [[nodiscard]]
    UIState GetState() const { return mState; }
    void SetTitle(const std::string& text, const Vector3& color = Color::White, int pointSize = 40);
    void AddButton(const std::string& name, const std::function<void()>& onClick);

protected:
    void DrawTexture(const Shader* shader, const Texture* texture, const Vector2& offset = Vector2::Zero, float scale = 1.0f);
    void SetRelativeMouseMode(bool relative);

    Game* mGame;
    Font* mFont;
    Texture* mTitle;
    Texture* mBackground;
    Texture* mButtonOn;
    Texture* mButtonOff;

    Vector2 mTitlePos;
    Vector2 mNextButtonPos;
    Vector2 mBGPos;

    UIState mState;
    std::vector<Button*> mButtons;
};
