#pragma once

#include "Component.h"
#include "Math.h"

class Actor;
class Mesh;
class Shader;

class PointLightComponent : public Component {
public:
    explicit PointLightComponent(Actor* owner);
    ~PointLightComponent() override;

    void Draw(const Shader* shader, const Mesh* mesh);

    Vector3 mDiffuseColor;
    float mInnerRadius;
    float mOuterRadius;
};
