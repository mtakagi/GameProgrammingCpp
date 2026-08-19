#pragma once

#include "Component.h"
#include "Math.h"

class Actor;

class CameraComponent : public Component {
public:
    explicit CameraComponent(Actor* owner, int updateOrder = 200);

protected:
    void SetViewMatrix(const Matrix4& view) const;
};
