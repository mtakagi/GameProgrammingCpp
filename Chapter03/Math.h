#pragma once

#include <cmath>

namespace Math {
    constexpr float Pi = 3.1415926535f;
    constexpr float TwoPi = Pi * 2.0f;
    constexpr float PiOver2 = Pi / 2.0f;

    inline float ToRadians(const float degrees) {
        return degrees * Pi / 180.0f;
    }

    inline float ToDegrees(const float radians) {
        return radians * 180.0f / Pi;
    }

    inline bool NearZero(const float val, const float epsilon = 0.001f) {
        if (fabs(val) <= epsilon) {
            return true;
        }

        return false;
    }

    inline float Cos(const float angle) {
        return cosf(angle);
    }

    inline float Sin(const float angle) {
        return sinf(angle);
    }
}

class Vector2 {
public:
    float x;
    float y;

    friend Vector2 operator+(const Vector2& a, const Vector2& b) {
        return Vector2(a.x + b.x, a.y + b.y);
    }

    friend Vector2 operator-(const Vector2& a, const Vector2& b) {
        return Vector2(a.x - b.x, a.y - b.y);
    }

    friend Vector2 operator*(const Vector2& a, const Vector2& b) {
        return Vector2(a.x * b.x, a.y * b.y);
    }

    friend Vector2 operator*(const Vector2& vector, const float scalar) {
        return Vector2(vector.x * scalar, vector.y * scalar);
    }

    friend Vector2 operator*(const float scalar, const Vector2& vector) {
        return Vector2{vector.x * scalar, vector.y * scalar};
    }

    Vector2& operator+=(const Vector2& vector) {
        x += vector.x;
        y += vector.y;

        return *this;
    }

    [[nodiscard]]
    float LengthSq() const {
        return x * x + y * y;
    }

    const static Vector2 Zero;
};
