#pragma once

namespace Math {
    constexpr float Pi = 3.1415926535f;

    inline float ToRadians(const float degrees) {
        return degrees * Pi / 180.0f;
    }

    inline float ToDegrees(const float radians) {
        return radians * 180.0f / Pi;
    }
}

class Vector2 {
public:
    float x;
    float y;

    const static Vector2 Zero;
};
