#pragma once

#include <vector>
#include "Math.h"

struct LineSegment {
    LineSegment(const Vector3& start, const Vector3& end);
    [[nodiscard]]
    Vector3 PointOnSegment(float t) const;
    [[nodiscard]]
    float MinDistSq(const Vector3& point) const;
    [[nodiscard]]
    static float MinDistSq(const LineSegment& s1, const LineSegment& s2);

    Vector3 mStart;
    Vector3 mEnd;
};

struct Plane {
    Plane(const Vector3& normal, float d);
    Plane(const Vector3& a, const Vector3& b, const Vector3& c);
    [[nodiscard]]
    float SignedDist(const Vector3& point) const;

    Vector3 mNormal;
    float mD;
};

struct Sphere {
    Sphere(const Vector3& center, float radius);
    [[nodiscard]]
    bool Contains(const Vector3& point) const;

    Vector3 mCenter;
    float mRadius;
};

struct AABB {
    AABB(const Vector3& min, const Vector3& max);
    void UpdateMinMax(const Vector3& point);
    void Rotate(const Quaternion& q);
    [[nodiscard]]
    bool Contains(const Vector3& point) const;
    [[nodiscard]]
    float MinDistSq(const Vector3& point) const;

    Vector3 mMin;
    Vector3 mMax;
};

struct OBB {
    Vector3 mCenter;
    Quaternion mRotation;
    Vector3 mExtents;
};

struct Capsule {
    Capsule(const Vector3& start, const Vector3& end, float radius);
    [[nodiscard]]
    Vector3 PointOnSegment(float t) const;
    [[nodiscard]]
    bool Contains(const Vector3& point) const;

    LineSegment mSegment;
    float mRadius;
};

struct ConvexPolygon {
    [[nodiscard]]
    bool Contains(const Vector2& point) const;

    std::vector<Vector2> mVertices;
};

[[nodiscard]]
bool Intersect(const Sphere& a, const Sphere& b);
[[nodiscard]]
bool Intersect(const AABB& a, const AABB& b);
[[nodiscard]]
bool Intersect(const Capsule& a, const Capsule& b);
[[nodiscard]]
bool Intersect(const Sphere& s, const AABB& box);

[[nodiscard]]
bool Intersect(const LineSegment& l, const Sphere& s, float& outT);
[[nodiscard]]
bool Intersect(const LineSegment& l, const Plane& p, float& outT);
[[nodiscard]]
bool Intersect(const LineSegment& l, const AABB& b, float& outT, Vector3& outNorm);

[[nodiscard]]
bool SweptSphere(const Sphere& P0, const Sphere& P1, const Sphere& Q0, const Sphere& Q1, float& outT);