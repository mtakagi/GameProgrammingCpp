#include <algorithm>
#include "BoxComponent.h"
#include "PhysWorld.h"

PhysWorld::PhysWorld(Game *game)
    : mGame(game){
}

bool PhysWorld::SegmentCast(const LineSegment &l, CollisionInfo &outColl, const Actor* ignore) const {
    auto collided = false;
    auto closestT = Math::Infinity;
    Vector3 norm;

    for (const auto& box : mBoxes) {
        if (ignore != nullptr && box->GetOwner() == ignore) {
            continue;
        }

        float t;

        if (Intersect(l, box->GetWorldBox(), t, norm)) {
            if (t < closestT) {
                closestT = t;
                outColl.mPoint = l.PointOnSegment(t);
                outColl.mNormal = norm;
                outColl.mBox = box;
                outColl.mActor = box->GetOwner();
                collided = true;
            }
        }
    }

    return collided;
}

void PhysWorld::TestPairwise(const std::function<void(Actor *, Actor *)>& f) const {
    for (size_t i = 0; i < mBoxes.size(); i++) {
        for (size_t j = i + 1; j < mBoxes.size(); j++) {
            const auto a = mBoxes[i];
            const auto b = mBoxes[j];

            if (Intersect(a->GetWorldBox(), b->GetWorldBox())) {
                f(a->GetOwner(), b->GetOwner());
            }
        }
    }
}

void PhysWorld::TestSweepAndPrune(const std::function<void(Actor *, Actor *)>& f) {
    std::ranges::sort(mBoxes, [](auto a, auto b) {
        return a->GetWorldBox().mMin.x < b->GetWorldBox().mMin.x;
    });

    for (size_t i = 0; i < mBoxes.size(); i++) {
        const auto a = mBoxes[i];
        const auto max = a->GetWorldBox().mMax.x;

        for (size_t j = i + 1; j < mBoxes.size(); j++) {
            const auto b = mBoxes[j];

            if (b->GetWorldBox().mMin.x > max) {
                break;
            }

            if (Intersect(a->GetWorldBox(), b->GetWorldBox())) {
                f(a->GetOwner(), b->GetOwner());
            }
        }
    }
}

void PhysWorld::AddBox(BoxComponent *box) {
    mBoxes.emplace_back(box);
}

void PhysWorld::RemoveBox(BoxComponent *box) {
    const auto iter = std::ranges::find(mBoxes, box);

    if (iter != mBoxes.end()) {
        std::iter_swap(iter, mBoxes.end() - 1);
        mBoxes.pop_back();
    }
}
