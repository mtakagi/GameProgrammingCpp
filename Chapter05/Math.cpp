#include "Math.h"

const Vector2 Vector2::Zero = {0.0f, 0.0f};
const Vector3 Vector3::Zero = {0.0f, 0.0f, 0.0f};

static float m4Ident[4][4] = {
    1.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 1.0f,
};

const Matrix4 Matrix4::Identity(m4Ident);