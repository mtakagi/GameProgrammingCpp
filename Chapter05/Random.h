#pragma once

#include <random>
#include "Math.h"

class Random {
public:
    [[nodiscard]]
    static float GetFloat();
    [[nodiscard]]
    static float GetFloatRange(float min, float max);
    [[nodiscard]]
    static Vector2 GetVector(const Vector2& min, const Vector2& max);

private:
    static std::mt19937 sGenerator;
};
