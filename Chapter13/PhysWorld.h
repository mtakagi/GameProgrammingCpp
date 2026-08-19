#pragma once

#include <functional>
#include <vector>
#include "Collision.h"
#include "Math.h"

class Actor;
class BoxComponent;
class Game;

class PhysWorld {
public:
    explicit PhysWorld(Game *game);

    struct CollisionInfo {
        Vector3 mPoint;
        Vector3 mNormal;
        BoxComponent* mBox{};
        Actor *mActor{};
    };

    bool SegmentCast(const LineSegment& l, CollisionInfo& outColl, const Actor* ignore = nullptr) const;

    void TestPairwise(const std::function<void(Actor*, Actor*)>& f) const;
    void TestSweepAndPrune(const std::function<void(Actor*, Actor*)>& f);

    void AddBox(BoxComponent* box);
    void RemoveBox(BoxComponent* box);

private:
    Game* mGame;
    std::vector<BoxComponent*> mBoxes;
};
