#include "Actor.h"
#include "BallActor.h"
#include "BallMove.h"
#include "Collision.h"
#include "Game.h"
#include "PhysWorld.h"
#include "TargetActor.h"

BallMove::BallMove(Actor* owner)
    : MoveComponent(owner)
    , mPlayer(nullptr) {
}

void BallMove::Update(const float deltaTime) {
    constexpr auto segmentLength = 30.0f;
    const auto start = mOwner->GetPosition();
    auto dir = mOwner->GetForward();
    const auto end = start + dir * segmentLength;

    const LineSegment l(start, end);

    const auto phys = mOwner->GetGame()->GetPhysWorld();
    PhysWorld::CollisionInfo info;

    if (phys->SegmentCast(l, info) && info.mActor != mPlayer) {
        dir = Vector3::Reflect(dir, info.mNormal);
        mOwner->RotateToNewForward(dir);

        if (dynamic_cast<TargetActor*>(info.mActor)) {
            if (const auto ball = dynamic_cast<BallActor*>(mOwner)) {
                ball->HitTarget();
            }
        }
    }

    MoveComponent::Update(deltaTime);
}
