#pragma once

#include "Math.h"

constexpr size_t MAX_SKELETON_BONES = 96;

struct MatrixPalette {
    Matrix4 mEntry[MAX_SKELETON_BONES];
};