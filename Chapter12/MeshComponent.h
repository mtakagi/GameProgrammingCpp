#pragma once

#include "Component.h"

class Actor;
class Shader;
class Mesh;

class MeshComponent : public Component {
public:
    explicit MeshComponent(Actor* owner, bool isSkeletal = false);
    ~MeshComponent() override;

    virtual void Draw(Shader* shader);
    virtual void SetMesh(Mesh* mesh) { mMesh = mesh; }
    void SetTextureIndex(const size_t index) { mTextureIndex = index; }

    void SetVisible(const bool visible) { mVisible = visible; }
    [[nodiscard]]
    bool GetVisible() const { return mVisible; }

    bool GetIsSkeletal() const { return mIsSkeletal; }
protected:
    Mesh* mMesh;
    size_t mTextureIndex;
    bool mVisible;
    bool mIsSkeletal;
};
