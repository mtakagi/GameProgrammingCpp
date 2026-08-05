#include "Random.h"

float Random::GetFloat() {
    return GetFloatRange(0.0f, 1.0f);
}

float Random::GetFloatRange(const float min, const float max) {
    std::uniform_real_distribution dist(min, max);

    return dist(sGenerator);
}

Vector2 Random::GetVector(const Vector2& min, const Vector2& max) {
    const auto r = Vector2(GetFloat(), GetFloat());

    return min + (max - min) * r;
}

std::mt19937 Random::sGenerator;