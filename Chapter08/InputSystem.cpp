#include <SDL.h>
#include <cstring>
#include "InputSystem.h"

bool KeyboardState::GetKeyValue(const SDL_Scancode keyCode) const {
    return mCurrState[keyCode] == 1;
}

ButtonState KeyboardState::GetKeyState(const SDL_Scancode keyCode) const {
    if (mPrevState[keyCode] == 0) {
        if (mCurrState[keyCode] == 0) {
            return ENone;
        }

        return EPressed;
    }

    if (mCurrState[keyCode] == 0) {
        return EReleased;
    }

    return EHeld;
}

bool MouseState::GetButtonValue(const int button) const {
    return (SDL_BUTTON(button) & mCurrButtons);
}

ButtonState MouseState::GetButtonState(const int button) const {
    const auto mask = SDL_BUTTON(button);

    if ((mask & mPrevButtons) == 0) {
        if ((mask & mCurrButtons) == 0) {
            return ENone;
        }

        return EPressed;
    }

    if ((mask & mCurrButtons) == 0) {
        return EReleased;
    }

    return EHeld;
}

bool ControllerState::GetButtonValue(const SDL_GameControllerButton button) const {
    return mCurrButtons[button] == 1;
}

ButtonState ControllerState::GetButtonState(const SDL_GameControllerButton button) const {
    if (mPrevButtons[button] == 0) {
        if (mCurrButtons[button] == 0) {
            return ENone;
        }

        return EPressed;
    }

    if (mCurrButtons[button] == 0) {
        return EReleased;
    }

    return EHeld;
}

bool InputSystem::Initialize() {
    mState.Keyboard.mCurrState = SDL_GetKeyboardState(nullptr);

    memset(mState.Keyboard.mPrevState, 0, SDL_NUM_SCANCODES);

    mState.Mouse.mCurrButtons = 0;
    mState.Mouse.mPrevButtons = 0;

    mController = SDL_GameControllerOpen(0);

    mState.Controller.mIsConnected = (mController != nullptr);

    if (!mState.Controller.mIsConnected) {
        SDL_Log("ゲームコントローラーを開けませんでした: %s", SDL_GetError());
    }

    memset(mState.Controller.mCurrButtons, 0, SDL_CONTROLLER_BUTTON_MAX);
    memset(mState.Controller.mPrevButtons, 0, SDL_CONTROLLER_BUTTON_MAX);

    return true;
}

void InputSystem::Shutdown() {
    if (mController) {
        SDL_GameControllerClose(mController);
        mController = nullptr;
    }
}

void InputSystem::PrepareForUpdate() {
    memcpy(mState.Keyboard.mPrevState, mState.Keyboard.mCurrState, SDL_NUM_SCANCODES);

    mState.Mouse.mPrevButtons = mState.Mouse.mCurrButtons;
    mState.Mouse.mIsRelative = false;
    mState.Mouse.mScrollWheel = Vector2::Zero;

    memcpy(mState.Controller.mPrevButtons, mState.Controller.mCurrButtons, SDL_CONTROLLER_BUTTON_MAX);
}

void InputSystem::Update() {
    auto x = 0, y = 0;

    if (mState.Mouse.mIsRelative) {
        mState.Mouse.mCurrButtons = SDL_GetRelativeMouseState(&x, &y);
    } else {
        mState.Mouse.mCurrButtons = SDL_GetMouseState(&x, &y);
    }

    mState.Mouse.mMousePos.x = static_cast<float>(x);
    mState.Mouse.mMousePos.y = static_cast<float>(y);

    for (auto i = 0; i < SDL_CONTROLLER_BUTTON_MAX; i++) {
        mState.Controller.mCurrButtons[i] = SDL_GameControllerGetButton(mController, static_cast<SDL_GameControllerButton>(i));
    }

    mState.Controller.mLeftTrigger = Filter1D(SDL_GameControllerGetAxis(mController, SDL_CONTROLLER_AXIS_TRIGGERLEFT));
    mState.Controller.mRightTrigger = Filter1D(SDL_GameControllerGetAxis(mController, SDL_CONTROLLER_AXIS_TRIGGERRIGHT));

    x = SDL_GameControllerGetAxis(mController, SDL_CONTROLLER_AXIS_LEFTX);
    y = SDL_GameControllerGetAxis(mController, SDL_CONTROLLER_AXIS_LEFTY);
    mState.Controller.mLeftStick = Filter2D(x, y);

    x = SDL_GameControllerGetAxis(mController, SDL_CONTROLLER_AXIS_RIGHTX);
    y = SDL_GameControllerGetAxis(mController, SDL_CONTROLLER_AXIS_RIGHTY);
    mState.Controller.mRightStick = Filter2D(x, y);
}

void InputSystem::ProcessEvent(const SDL_Event &event) {
    switch (event.type) {
        case SDL_MOUSEWHEEL:
            mState.Mouse.mScrollWheel = Vector2(static_cast<float>(event.wheel.x), static_cast<float>(event.wheel.y));
            break;
        default:
            break;
    }
}

void InputSystem::SetRelativeMouseMode(const bool value) {
    const auto set = value ? SDL_TRUE : SDL_FALSE;
    SDL_SetRelativeMouseMode(set);

    mState.Mouse.mIsRelative = set;
}

float InputSystem::Filter1D(const int input) {
    constexpr auto deadZone = 250;
    constexpr auto maxValue = 30000;
    auto retVal = 0.0f;
    auto absValue = input > 0 ? input : -input;

    if (absValue > deadZone) {
        retVal = static_cast<float>(absValue - deadZone) / (maxValue - deadZone);
        retVal = input > 0 ? retVal : -1.0f * retVal;
        retVal = Math::Clamp(retVal, -1.0f, 1.0f);
    }

    return retVal;
}

Vector2 InputSystem::Filter2D(const int inputX, const int inputY) {
    constexpr auto deadZone = 8000.0f;
    constexpr auto maxValue = 30000.0f;
    Vector2 dir;
    dir.x = static_cast<float>(inputX);
    dir.y = static_cast<float>(inputY);
    const auto length = dir.Length();

    if (length < deadZone) {
        dir = Vector2::Zero;
    } else {
        auto f = (length - deadZone) / (maxValue - deadZone);
        f = Math::Clamp(f, 0.0f, 1.0f);
        dir *= f / length;
    }

    return dir;
}