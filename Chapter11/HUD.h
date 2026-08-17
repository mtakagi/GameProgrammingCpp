#pragma once

#include <vector>
#include "UIScreen.h"

class Game;
class Shader;
class TargetComponent;
class Texture;

class HUD : public UIScreen {
public:
    explicit HUD(Game* game);
    ~HUD() override;

    void Update(float deltaTime) override;
    void Draw(Shader* shader) override;

    void AddTargetComponent(TargetComponent* target);
    void RemoveTargetComponent(TargetComponent* target);

protected:
    void UpdateCrosshair(float deltaTime);
    void UpdateRadar(float deltaTime);

    Texture* mHealthBar;
    Texture* mRadar;
    Texture* mCrosshair;
    Texture* mCrosshairEnemy;
    Texture* mBlipTex;
    Texture* mRadarArrow;

    std::vector<TargetComponent*> mTargetComps;
    std::vector<Vector2> mBlips;

    float mRadarRange;
    float mRadarRadius;
    bool mTargetEnemy;
};
