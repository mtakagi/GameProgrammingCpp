#include <algorithm>
#include "Actor.h"
#include "FollowActor.h"
#include "HUD.h"
#include "Game.h"
#include "PhysWorld.h"
#include "Renderer.h"
#include "TargetComponent.h"

HUD::HUD(Game* game)
    : UIScreen(game)
    , mRadarRange(2000.0f)
    , mRadarRadius(92.0f)
    , mTargetEnemy(false) {
    const auto r = mGame->GetRenderer();

    mHealthBar = r->GetTexture("Assets/HealthBar.png");
    mRadar = r->GetTexture("Assets/Radar.png");
    mCrosshair = r->GetTexture("Assets/Crosshair.png");
    mCrosshairEnemy = r->GetTexture("Assets/CrosshairRed.png");
    mBlipTex = r->GetTexture("Assets/Blip.png");
    mRadarArrow = r->GetTexture("Assets/RadarArrow.png");
}

HUD::~HUD() = default;

void HUD::Update(const float deltaTime) {
    UIScreen::Update(deltaTime);

    UpdateCrosshair(deltaTime);
    UpdateRadar(deltaTime);
}

void HUD::Draw(Shader* shader) {
    const auto cross = mTargetEnemy ? mCrosshairEnemy : mCrosshair;
    DrawTexture(shader, cross, Vector2::Zero, 2.0f);

    const Vector2 cRadarPos(-390.0f, 275.0f);
    DrawTexture(shader, mRadar, cRadarPos, 1.0f);

    for (const auto& blip : mBlips) {
        DrawTexture(shader, mBlipTex, cRadarPos + blip, 1.0f);
    }

    DrawTexture(shader, mRadarArrow, cRadarPos);

    DrawTexture(shader, mHealthBar, Vector2(-350.0f, -350.0f));

    const auto mirror = mGame->GetRenderer()->GetMirrorTexture();
    DrawTexture(shader, mirror, Vector2(-350.0f, -250.0f), 1.0f, true);
}

void HUD::AddTargetComponent(TargetComponent *target) {
    mTargetComps.emplace_back(target);
}

void HUD::RemoveTargetComponent(TargetComponent *target) {
    const auto iter = std::ranges::find(mTargetComps, target);
    mTargetComps.erase(iter);
}

void HUD::UpdateCrosshair(const float deltaTime) {
    mTargetEnemy = false;
    constexpr auto cAimDist = 5000.0f;
    Vector3 start, dir;

    mGame->GetRenderer()->GetScreenDirection(start, dir);
    const LineSegment l(start, start + dir * cAimDist);
    PhysWorld::CollisionInfo info;

    if (mGame->GetPhysWorld()->SegmentCast(l, info, mGame->GetPlayer())) {
        for (const auto& tc : mTargetComps) {
            if (tc->GetOwner() == info.mActor) {
                mTargetEnemy = true;
                break;
            }
        }
    }
}

void HUD::UpdateRadar(const float deltaTime) {
    mBlips.clear();

    const auto playerPos = mGame->GetPlayer()->GetPosition();
    const Vector2 playerPos2D(playerPos.y, playerPos.x);
    const auto playerForward = mGame->GetPlayer()->GetForward();
    const Vector2 playerForward2D(playerForward.x, playerForward.y);
    const auto angle = Math::Atan2(playerForward2D.y, playerForward2D.x);
    const auto rotMat = Matrix3::CreateRotation(angle);

    for (const auto& tc : mTargetComps) {
        const auto targetPos = tc->GetOwner()->GetPosition();
        Vector2 actorPos2D(targetPos.y, targetPos.x);

        const auto playerToTarget = actorPos2D - playerPos2D;

        if (playerToTarget.LengthSq() <= mRadarRange * mRadarRange) {
            auto blipPos = playerToTarget;
            blipPos *= mRadarRadius / mRadarRange;

            blipPos = Vector2::Transform(blipPos, rotMat);
            mBlips.emplace_back(blipPos);
        }
    }
}