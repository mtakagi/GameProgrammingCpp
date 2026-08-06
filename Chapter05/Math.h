#pragma once

#include <cmath>
#include <cstring>

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

    inline float Atan2(const float y, const float x) {
        return atan2f(y, x);
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

    [[nodiscard]]
    float Length() const {
        return sqrtf(x * x + y * y);
    }

    const static Vector2 Zero;
};

class Vector3 {
public:
    float x;
    float y;
    float z;

    friend Vector3 operator+(const Vector3& a, const Vector3& b) {
        return Vector3(a.x + b.x, a.y + b.y, a.z + b.z);
    }

    friend Vector3 operator-(const Vector3& a, const Vector3& b) {
        return Vector3(a.x - b.x, a.y - b.y, a.z - b.z);
    }

    friend Vector3 operator*(const Vector3& a, const Vector3& b) {
        return Vector3(a.x * b.x, a.y * b.y, a.z * b.z);
    }

    friend Vector3 operator*(const Vector3& vector, const float scalar) {
        return Vector3(vector.x * scalar, vector.y * scalar, vector.z * scalar);
    }

    friend Vector3 operator*(const float scalar, const Vector3& vector) {
        return Vector3{vector.x * scalar, vector.y * scalar, vector.z * scalar};
    }

    Vector3& operator+=(const Vector3& vector) {
        x += vector.x;
        y += vector.y;
        z += vector.z;

        return *this;
    }

    [[nodiscard]]
    float LengthSq() const {
        return x * x + y * y + z * z;
    }

    [[nodiscard]]
    float Length() const {
        return sqrtf(x * x + y * y + z * z);
    }

    const static Vector3 Zero;
};

class Matrix4 {
public:
    float mat[4][4];

    Matrix4() : mat() {
        *this = Identity;
    }

    explicit Matrix4(const float inMat[4][4]) : mat() {
        memcpy(mat, inMat, 16 * sizeof(float));
    }

    [[nodiscard]]
    const float* GetAsFloatPtr() const {
        return &mat[0][0];
    }

    friend Matrix4 operator*(const Matrix4& a, const Matrix4& b) {
        Matrix4 retVal;

        retVal.mat[0][0] =
            a.mat[0][0] * b.mat[0][0] +
            a.mat[0][1] * b.mat[1][0] +
            a.mat[0][2] * b.mat[2][0] +
            a.mat[0][3] * b.mat[3][0];

        retVal.mat[0][1] =
            a.mat[0][0] * b.mat[0][1] +
            a.mat[0][1] * b.mat[1][1] +
            a.mat[0][2] * b.mat[2][1] +
            a.mat[0][3] * b.mat[3][1];

        retVal.mat[0][2] =
            a.mat[0][0] * b.mat[0][2] +
            a.mat[0][1] * b.mat[1][2] +
            a.mat[0][2] * b.mat[2][2] +
            a.mat[0][3] * b.mat[3][2];

        retVal.mat[0][3] =
            a.mat[0][0] * b.mat[0][3] +
            a.mat[0][1] * b.mat[1][3] +
            a.mat[0][2] * b.mat[2][3] +
            a.mat[0][3] * b.mat[3][3];

        retVal.mat[1][0] =
            a.mat[1][0] * b.mat[0][0] +
            a.mat[1][1] * b.mat[1][0] +
            a.mat[1][2] * b.mat[2][0] +
            a.mat[1][3] * b.mat[3][0];

        retVal.mat[1][1] =
            a.mat[1][0] * b.mat[0][1] +
            a.mat[1][1] * b.mat[1][1] +
            a.mat[1][2] * b.mat[2][1] +
            a.mat[1][3] * b.mat[3][1];

        retVal.mat[1][2] =
            a.mat[1][0] * b.mat[0][2] +
            a.mat[1][1] * b.mat[1][2] +
            a.mat[1][2] * b.mat[2][2] +
            a.mat[1][3] * b.mat[3][2];

        retVal.mat[1][3] =
            a.mat[1][0] * b.mat[0][3] +
            a.mat[1][1] * b.mat[1][3] +
            a.mat[1][2] * b.mat[2][3] +
            a.mat[1][3] * b.mat[3][3];

        retVal.mat[2][0] =
            a.mat[2][0] * b.mat[0][0] +
            a.mat[2][1] * b.mat[1][0] +
            a.mat[2][2] * b.mat[2][0] +
            a.mat[2][3] * b.mat[3][0];

        retVal.mat[2][1] =
            a.mat[2][0] * b.mat[0][1] +
            a.mat[2][1] * b.mat[1][1] +
            a.mat[2][2] * b.mat[2][1] +
            a.mat[2][3] * b.mat[3][1];

        retVal.mat[2][2] =
            a.mat[2][0] * b.mat[0][2] +
            a.mat[2][1] * b.mat[1][2] +
            a.mat[2][2] * b.mat[2][2] +
            a.mat[2][3] * b.mat[3][2];

        retVal.mat[2][3] =
            a.mat[2][0] * b.mat[0][3] +
            a.mat[2][1] * b.mat[1][3] +
            a.mat[2][2] * b.mat[2][3] +
            a.mat[2][3] * b.mat[3][3];

        retVal.mat[3][0] =
            a.mat[3][0] * b.mat[0][0] +
            a.mat[3][1] * b.mat[1][0] +
            a.mat[3][2] * b.mat[2][0] +
            a.mat[3][3] * b.mat[3][0];

        retVal.mat[3][1] =
            a.mat[3][0] * b.mat[0][1] +
            a.mat[3][1] * b.mat[1][1] +
            a.mat[3][2] * b.mat[2][1] +
            a.mat[3][3] * b.mat[3][1];

        retVal.mat[3][2] =
            a.mat[3][0] * b.mat[0][2] +
            a.mat[3][1] * b.mat[1][2] +
            a.mat[3][2] * b.mat[2][2] +
            a.mat[3][3] * b.mat[3][2];

        retVal.mat[3][3] =
            a.mat[3][0] * b.mat[0][3] +
            a.mat[3][1] * b.mat[1][3] +
            a.mat[3][2] * b.mat[2][3] +
            a.mat[3][3] * b.mat[3][3];

        return retVal;
    }

    Matrix4& operator*=(const Matrix4& matrix) {
        *this = *this * matrix;

        return *this;
    }

    static Matrix4 CreateScale(const float x, const float y, const float z) {
        const float temp[4][4] = {
            { x, 0.0f, 0.0f, 0.0f },
            { 0.0f, y, 0.0f, 0.0f },
            { 0.0f, 0.0f, z, 0.0f },
            { 0.0f, 0.0f, 0.0f, 1.0f },
        };

        return Matrix4{temp};
    }

    static Matrix4 CreateScale(const Vector3& scale) {
        return CreateScale(scale.x, scale.y, scale.z);
    }

    static Matrix4 CreateScale(const float scale) {
        return CreateScale(scale, scale, scale);
    }

    static Matrix4 CreateRotationZ(const float theta) {
        const float temp[4][4] = {
            { Math::Cos(theta), Math::Sin(theta), 0.0f, 0.0f },
            { -Math::Sin(theta), Math::Cos(theta), 0.0f, 0.0f },
            { 0.0f, 0.0f, 1.0f, 0.0f },
            { 0.0f, 0.0f, 0.0f, 1.0f },
        };

        return Matrix4{temp};
    }

    static Matrix4 CreateTranslation(const Vector3& translation) {
        const float temp[4][4] = {
            { 1.0f, 0.0f, 0.0f, 0.0f },
            { 0.0f, 1.0f, 0.0f, 0.0f },
            { 0.0f, 0.0f, 1.0f, 0.0f },
            { translation.x, translation.y, translation.z, 1.0f },
        };

        return Matrix4{temp};
    }

    static Matrix4 CreateSimpleViewProj(const float width, const float height) {
        const float temp[4][4] = {
            { 2.0f / width, 0.0f, 0.0f, 0.0f },
            { 0.0f, 2.0f / height, 0.0f, 0.0f },
            { 0.0f, 0.0f, 1.0f, 0.0f },
            { 0.0f, 0.0f, 1.0f, 1.0f },
        };

        return Matrix4{temp};
    }

    const static Matrix4 Identity;
};