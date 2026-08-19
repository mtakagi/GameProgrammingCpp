#include <algorithm>
#include <array>
#include "Collision.h"

LineSegment::LineSegment(const Vector3 &start, const Vector3 &end)
    : mStart(start)
    , mEnd(end){
}

Vector3 LineSegment::PointOnSegment(const float t) const {
    return mStart + (mEnd - mStart) * t;
}

float LineSegment::MinDistSq(const Vector3 &point) const {
    const auto ab = mEnd - mStart;
    const auto ba = -1.0f * ab;
    const auto ac = point - mStart;
    const auto bc = point - mEnd;

    if (Vector3::Dot(ab, ac) < 0.0f) {
        return ac.LengthSq();
    }

    if (Vector3::Dot(ba, bc) < 0.0f) {
        return bc.LengthSq();
    }

    const auto scalar = Vector3::Dot(ac, ab) / Vector3::Dot(ab, ab);
    const auto p = scalar * ab;

    return (ac - p).LengthSq();
}

float LineSegment::MinDistSq(const LineSegment &s1, const LineSegment &s2) {
    const auto u = s1.mEnd - s1.mStart;
    const auto v = s2.mEnd - s2.mStart;
    const auto w = s1.mStart - s2.mStart;
    const auto a = Vector3::Dot(u, u);
    const auto b = Vector3::Dot(u, v);
    const auto c = Vector3::Dot(v, v);
    const auto d = Vector3::Dot(u, w);
    const auto e = Vector3::Dot(v, w);
    const auto D = a * c - b * b;
    float sN, sD = D;
    float tN, tD = D;

    if (Math::NearZero(D)) {
        sN = 0.0f;
        sD = 1.0f;
        tN = e;
        tD = c;
    } else {
        sN = b * e - c * d;
        tN = a * e - b * d;

        if (sN < 0.0f) {
            sN = 0.0f;
            tN = e;
            tD = c;
        } else if (sN > sD) {
            sN = sD;
            tN = e + b;
            tD = c;
        }
    }

    if (tN < 0.0f) {
        tN = 0.0f;

        if (-d < 0.0f) {
            sN = 0.0f;
        } else if (-d > a) {
            sN = sD;
        } else {
            sN = -d;
            sD = a;
        }
    } else if (tN > tD) {
        tN = tD;

        if ((-d + b) < 0.0f) {
            sN = 0.0f;
        } else if ((-d + b) > a) {
            sN = sD;
        } else {
            sN = -d + b;
            sD = a;
        }
    }

    const auto sc = Math::NearZero(sN) ? 0.0f : sN / sD;
    const auto tc = Math::NearZero(tN) ? 0.0f : tN / tD;
    const auto dP = w + (sc * u) - (tc * v);

    return dP.LengthSq();
}

Plane::Plane(const Vector3 &normal, const float d)
    : mNormal(normal)
    , mD(d) {
}

Plane::Plane(const Vector3 &a, const Vector3 &b, const Vector3 &c) {
    const auto ab = b - a;
    const auto ac = c - a;

    mNormal = Vector3::Cross(ab, ac);
    mNormal.Normalize();
    mD = -Vector3::Dot(a, mNormal);
}

float Plane::SignedDist(const Vector3 &point) const {
    return Vector3::Dot(point, mNormal) - mD;
}

Sphere::Sphere(const Vector3 &center, const float radius)
    : mCenter(center)
    , mRadius(radius) {
}

bool Sphere::Contains(const Vector3 &point) const {
    const auto distSq = (mCenter - point).LengthSq();

    return distSq <= (mRadius * mRadius);
}

AABB::AABB(const Vector3 &min, const Vector3 &max)
    : mMin(min)
    , mMax(max) {
}

void AABB::UpdateMinMax(const Vector3 &point) {
    mMin.x = Math::Min(mMin.x, point.x);
    mMin.y = Math::Min(mMin.y, point.y);
    mMin.z = Math::Min(mMin.z, point.z);

    mMax.x = Math::Max(mMax.x, point.x);
    mMax.y = Math::Max(mMax.y, point.y);
    mMax.z = Math::Max(mMax.z, point.z);
}

void AABB::Rotate(const Quaternion& q) {
    std::array<Vector3, 8> points;

    points[0] = mMin;
    points[1] = Vector3(mMax.x, mMin.y, mMin.z);
    points[2] = Vector3(mMin.x, mMax.y, mMin.z);
    points[3] = Vector3(mMin.x, mMin.y, mMax.z);

    points[4] = Vector3(mMin.x, mMax.y, mMax.z);
    points[5] = Vector3(mMax.x, mMin.y, mMax.z);
    points[6] = Vector3(mMax.x, mMax.y, mMin.z);
    points[7] = Vector3(mMax);

    auto p = Vector3::Transform(points[0], q);

    mMin = p;
    mMax = p;

    for (const auto& point : points) {
        p = Vector3::Transform(point, q);
        UpdateMinMax(p);
    }
}

bool AABB::Contains(const Vector3 &point) const {
    const auto outside = point.x < mMin.x ||
        point.y < mMin.y ||
        point.z < mMin.z ||
        point.x > mMax.x ||
        point.y > mMax.y ||
        point.z > mMax.z;

    return !outside;
}

float AABB::MinDistSq(const Vector3 &point) const {
    auto dx = Math::Max(mMin.x - point.x, 0.0f);
    dx = Math::Max(dx, point.x - mMax.x);
    auto dy = Math::Max(mMin.y - point.y, 0.0f);
    dy = Math::Max(dy, point.y - mMax.y);
    auto dz = Math::Max(mMin.z - point.z, 0.0f);
    dz = Math::Max(dz, point.z - mMax.z);

    return dx * dx + dy * dy + dz * dz;
}

Capsule::Capsule(const Vector3 &start, const Vector3 &end, const float radius)
    : mSegment(start, end)
    , mRadius(radius) {
}

Vector3 Capsule::PointOnSegment(const float t) const {
    return mSegment.PointOnSegment(t);
}

bool Capsule::Contains(const Vector3 &point) const {
    const auto distSq = mSegment.MinDistSq(point);

    return distSq <= (mRadius * mRadius);
}

bool ConvexPolygon::Contains(const Vector2 &point) const {
    auto sum = 0.0f;
    Vector2 a, b;

    for (size_t i = 0; i < mVertices.size() - 1; i++) {
        a = mVertices[i] - point;
        a.Normalize();
        b = mVertices[i + 1] - point;
        b.Normalize();

        sum += Math::Acos(Vector2::Dot(a, b));
    }

    a = mVertices.back() - point;
    a.Normalize();
    b = mVertices.front() - point;
    b.Normalize();
    sum += Math::Acos(Vector2::Dot(a, b));

    return Math::NearZero(sum - Math::TwoPi);
}

bool Intersect(const Sphere& a, const Sphere& b) {
    const auto distSq = (a.mCenter - b.mCenter).LengthSq();
    const auto sumRadii = a.mRadius + b.mRadius;

    return distSq <= (sumRadii * sumRadii);
}

bool Intersect(const AABB &a, const AABB &b) {
    const auto no = a.mMax.x < b.mMin.x ||
        a.mMax.y < b.mMin.y ||
        a.mMax.z < b.mMin.z ||
        b.mMax.x < a.mMin.x ||
        b.mMax.y < a.mMin.y ||
        b.mMax.z < a.mMin.z;

    return !no;
}

bool Intersect(const Capsule &a, const Capsule &b) {
    const auto distSq = LineSegment::MinDistSq(a.mSegment, b.mSegment);
    const auto sumRadii = a.mRadius + b.mRadius;

    return distSq <= (sumRadii * sumRadii);
}

bool Intersect(const Sphere &s, const AABB &box) {
    const auto distSq = box.MinDistSq(s.mCenter);

    return distSq <= (s.mRadius * s.mRadius);
}

bool Intersect(const LineSegment &l, const Sphere &s, float &outT) {
    const auto X = l.mStart - s.mCenter;
    const auto Y = l.mEnd - l.mStart;
    const auto a = Vector3::Dot(Y, Y);
    const auto b = 2.0f * Vector3::Dot(X, Y);
    const auto c = Vector3::Dot(X, X) - s.mRadius * s.mRadius;
    auto disc = b * b - 4.0f * a * c;

    if (disc < 0.0f) {
        return false;
    }

    disc = Math::Sqrt(disc);

    const auto tMin = (-b - disc) / (2.0f * a);
    const auto tMax = (-b + disc) / (2.0f * a);

    if (tMin >= 0.0f && tMin <= 1.0f) {
        outT = tMin;

        return true;
    }

    if (tMax >= 0.0f && tMax <= 1.0f) {
        outT = tMax;

        return true;
    }

    return false;
}

bool Intersect(const LineSegment &l, const Plane &p, float &outT) {
    const auto denom = Vector3::Dot(l.mEnd - l.mStart, p.mNormal);

    if (Math::NearZero(denom)) {
        if (Math::NearZero(Vector3::Dot(l.mStart, p.mNormal) - p.mD)) {
            return true;
        }

        return false;
    }

    const auto numer = -Vector3::Dot(l.mStart, p.mNormal) - p.mD;
    outT = numer / denom;

    if (outT >= 0.0f && outT <= 1.0f) {
        return true;
    }

    return false;
}

static bool TestSidePlane(const float start, const float end, const float negd, const Vector3& norm, std::vector<std::pair<float, Vector3>>& out) {
    const auto denom = end - start;

    if (Math::NearZero(denom)) {
        return false;
    }

    const auto numer = -start + negd;
    const auto t = numer / denom;

    if (t >= 0.0f && t <= 1.0f) {
        out.emplace_back(t, norm);

        return true;
    }

    return false;
}

bool Intersect(const LineSegment &l, const AABB &b, float &outT, Vector3 &outNorm) {
    std::vector<std::pair<float, Vector3>> tValues;

    TestSidePlane(l.mStart.x, l.mEnd.x, b.mMin.x, Vector3::NegUnitX, tValues);
    TestSidePlane(l.mStart.x, l.mEnd.x, b.mMax.x, Vector3::UnitX, tValues);
    TestSidePlane(l.mStart.y, l.mEnd.y, b.mMin.y, Vector3::NegUnitY, tValues);
    TestSidePlane(l.mStart.y, l.mEnd.y, b.mMax.y, Vector3::UnitY, tValues);
    TestSidePlane(l.mStart.z, l.mEnd.z, b.mMin.z, Vector3::NegUnitZ, tValues);
    TestSidePlane(l.mStart.z, l.mEnd.z, b.mMax.z, Vector3::UnitZ, tValues);

    std::ranges::sort(tValues, [](auto a, auto b) {
        return a.first < b.first;
    });

    for (const auto& t : tValues) {
        const auto point = l.PointOnSegment(t.first);

        if (b.Contains(point)) {
            outT = t.first;
            outNorm = t.second;

            return true;
        }
    }

    return false;
}

bool SweptSphere(const Sphere &P0, const Sphere &P1, const Sphere &Q0, const Sphere &Q1, float &outT) {
    const auto X = P0.mCenter - Q0.mCenter;
    const auto Y = P1.mCenter - P0.mCenter - (Q1.mCenter - Q0.mCenter);
    const auto a = Vector3::Dot(Y, Y);
    const auto b = 2.0f * Vector3::Dot(X, Y);
    const auto sumRadii = P0.mRadius + Q0.mRadius;
    const auto c = Vector3::Dot(X, X) - sumRadii * sumRadii;
    auto disc = b * b - 4.0f * a * c;

    if (disc < 0.0f) {
        return false;
    }

    disc = Math::Sqrt(disc);
    outT = (-b - disc) / (2.0f * a);

    // 書籍・リポジトリのママ
    if (outT >= 0.0f && outT <= 0.0f) {
        return true;
    }

    return false;
}
