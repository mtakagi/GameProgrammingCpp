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

    template<typename T>
    T Max(const T& a, const T& b) {
        return a > b ? a : b;
    }

    template<typename T>
    T Min(const T& a, const T& b) {
        return a < b ? a : b;
    }

    template<typename T>
    T Clamp(const T& value, const T& lower, const T& upper) {
        return Min(upper, Max(lower, value));
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

    inline float Cot(const float angle) {
        return 1.0f / tanf(angle);
    }

    inline float Sqrt(const float x) {
        return sqrtf(x);
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

    Vector2& operator*=(const float scalar) {
        x *= scalar;
        y *= scalar;

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

    Vector3() : x(0.0f), y(0.0f), z(0.0f) {}
    Vector3(const float x, const float y, const float z) : x(x), y(y), z(z) {}

    [[nodiscard]]
    const float* GetAsFloatPtr() const {
        return &x;
    }

    friend Vector3 operator+(const Vector3& a, const Vector3& b) {
        return Vector3 {a.x + b.x, a.y + b.y, a.z + b.z };
    }

    friend Vector3 operator-(const Vector3& a, const Vector3& b) {
        return Vector3 {a.x - b.x, a.y - b.y, a.z - b.z };
    }

    friend Vector3 operator*(const Vector3& a, const Vector3& b) {
        return Vector3 {a.x * b.x, a.y * b.y, a.z * b.z };
    }

    friend Vector3 operator*(const Vector3& vector, const float scalar) {
        return Vector3 { vector.x * scalar, vector.y * scalar, vector.z * scalar };
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

    void Normalize() {
        const auto length = Length();
        x /= length;
        y /= length;
        z /= length;
    }

    [[nodiscard]]
    float LengthSq() const {
        return x * x + y * y + z * z;
    }

    [[nodiscard]]
    float Length() const {
        return sqrtf(x * x + y * y + z * z);
    }

    static Vector3 Normalize(const Vector3& vec) {
        auto temp = vec;
        temp.Normalize();

        return temp;
    }

    static float Dot(const Vector3& a, const Vector3& b) {
        return a.x * b.x + a.y * b.y + a.z * b.z;
    }

    static Vector3 Cross(const Vector3& a, const Vector3& b) {
        Vector3 temp;
        temp.x = a.y * b.z - a.z * b.y;
        temp.y = a.z * b.x - a.x * b.z;
        temp.z = a.x * b.y - a.y * b.x;

        return temp;
    }

    static Vector3 Transform(const Vector3& v, const class Quaternion& q);

    const static Vector3 Zero;
    const static Vector3 UnitX;
    const static Vector3 UnitY;
    const static Vector3 UnitZ;
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

    [[nodiscard]]
    Vector3 GetTranslation() const {
        return Vector3{ mat[3][0], mat[3][1], mat[3][2] };
    }

    void Invert();

    [[nodiscard]]
    Vector3 GetXAxis() const {
        return Vector3::Normalize(Vector3(mat[0][0], mat[0][1], mat[0][2]));
    }

    [[nodiscard]]
    Vector3 GetYAxis() const {
        return Vector3::Normalize(Vector3(mat[1][0], mat[1][1], mat[1][2]));
    }

    [[nodiscard]]
    Vector3 GetZAxis() const {
        return Vector3::Normalize(Vector3(mat[2][0], mat[2][1], mat[2][2]));
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

    static Matrix4 CreateFromQuaternion(const Quaternion& q);

    static Matrix4 CreateLookAt(const Vector3& eye, const Vector3& target, const Vector3& up) {
        const auto zaxis = Vector3::Normalize(target - eye);
        const auto xaxis = Vector3::Normalize(Vector3::Cross(up, zaxis));
        const auto yaxis = Vector3::Normalize(Vector3::Cross(zaxis, xaxis));
        Vector3 trans;
        trans.x = -Vector3::Dot(xaxis, eye);
        trans.y = -Vector3::Dot(yaxis, eye);
        trans.z = -Vector3::Dot(zaxis, eye);

        const float temp[4][4] = {
            { xaxis.x, yaxis.x, zaxis.x, 0.0f },
            { xaxis.y, yaxis.y, zaxis.y, 0.0f },
            { xaxis.z, yaxis.z, zaxis.z, 0.0f },
            { trans.x, trans.y, trans.z, 1.0f },
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

    static Matrix4 CreatePerspectiveFOV(const float fovY, const float width, const float height, const float near, const float far) {
        const auto yScale = Math::Cot(fovY / 2.0f);
        const auto xScale = yScale * height / width;
        const float temp[4][4] = {
            { xScale, 0.0f, 0.0f, 0.0f },
            { 0.0f, yScale, 0.0f, 0.0f },
            { 0.0f, 0.0f, far / (far - near), 1.0f },
            { 0.0f, 0.0f, -near * far / (far - near), 0.0f },
        };

        return Matrix4{temp};
    }

    const static Matrix4 Identity;
};

class Quaternion {
public:
    float x;
    float y;
    float z;
    float w;

    Quaternion() : x(), y(), z(), w() {
        *this = Identity;
    }

    Quaternion(const float x, const float y, const float z, const float w) : x(x), y(y), z(z), w(w) {}

    Quaternion(const Vector3& axis, const float angle) {
        const auto scalar = Math::Sin(angle / 2.0f);
        x = axis.x * scalar;
        y = axis.y * scalar;
        z = axis.z * scalar;
        w = Math::Cos(angle / 2.0f);
    }

    static Quaternion Concatenate(const Quaternion& q, const Quaternion& p) {
        Quaternion retVal;
        const Vector3 qv(q.x, q.y, q.z);
        const Vector3 pv(p.x, p.y, p.z);
        const Vector3 newVec = p.w * qv + q.w * pv + Vector3::Cross(pv, qv);

        retVal.x = newVec.x;
        retVal.y = newVec.y;
        retVal.z = newVec.z;
        retVal.w = p.w * q.w - Vector3::Dot(pv, qv);

        return retVal;
    }

    const static Quaternion Identity;
};