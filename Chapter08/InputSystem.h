#pragma once

#include <SDL_events.h>
#include <SDL_gamecontroller.h>
#include <SDL_scancode.h>
#include "Math.h"

enum ButtonState {
    ENone,
    EPressed,
    EReleased,
    EHeld,
};

class KeyboardState {
public:
    friend class InputSystem;
    [[nodiscard]]
    bool GetKeyValue(SDL_Scancode keyCode) const;
    [[nodiscard]]
    ButtonState GetKeyState(SDL_Scancode keyCode) const;

private:
    const Uint8* mCurrState = nullptr;
    Uint8 mPrevState[SDL_NUM_SCANCODES] = {};
};

class MouseState {
public:
    friend class InputSystem;

    [[nodiscard]]
    const Vector2& GetPosition() const { return mMousePos; }
    [[nodiscard]]
    const Vector2& GetScrollWheel() const { return mScrollWheel; }
    [[nodiscard]]
    bool IsRelative() const { return mIsRelative; }

    [[nodiscard]]
    bool GetButtonValue(int button) const;
    [[nodiscard]]
    ButtonState GetButtonState(int button) const;

private:
    Vector2 mMousePos = { .x = 0.0f, .y = 0.0f };
    Vector2 mScrollWheel = { .x = 0.0f, .y = 0.0f };
    Uint32 mCurrButtons = 0;
    Uint32 mPrevButtons = 0;
    bool mIsRelative = false;
};

class ControllerState {
public:
    friend class InputSystem;

    [[nodiscard]]
    bool GetButtonValue(SDL_GameControllerButton button) const;
    [[nodiscard]]
    ButtonState GetButtonState(SDL_GameControllerButton button) const;

    [[nodiscard]]
    const Vector2& GetLeftStick() const { return mLeftStick; }
    [[nodiscard]]
    const Vector2& GetRightStick() const { return mRightStick; }
    [[nodiscard]]
    float GetLeftTrigger() const { return mLeftTrigger; }
    [[nodiscard]]
    float GetRightTrigger() const { return mRightTrigger; }

    [[nodiscard]]
    bool GetIsConnected() const { return mIsConnected; }

private:
    Uint8 mCurrButtons[SDL_CONTROLLER_BUTTON_MAX] = {};
    Uint8 mPrevButtons[SDL_CONTROLLER_BUTTON_MAX] = {};

    Vector2 mLeftStick = { .x = 0.0f, .y = 0.0f };
    Vector2 mRightStick = { .x = 0.0f, .y = 0.0f };
    float mLeftTrigger = 0.0f;
    float mRightTrigger = 0.0f;
    bool mIsConnected = false;
};

struct InputState {
    KeyboardState Keyboard;
    MouseState Mouse;
    ControllerState Controller;
};

class InputSystem {
public:
    bool Initialize();
    void Shutdown();

    void PrepareForUpdate();
    void Update();
    void ProcessEvent(const SDL_Event& event);

    [[nodiscard]]
    const InputState& GetState() const { return mState; }

    void SetRelativeMouseMode(bool value);

private:
    float Filter1D(int input);
    Vector2 Filter2D(int inputX, int inputY);

    InputState mState;
    SDL_GameController* mController = nullptr;
};
