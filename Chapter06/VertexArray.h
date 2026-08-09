#pragma once

class VertexArray {
public:
    explicit VertexArray(const float* verts, unsigned int numVerts, const unsigned int* indices, unsigned int numIndices);
    ~VertexArray();

    void SetActive() const;

    [[nodiscard]]
    unsigned int GetNumIndices() const { return mNumIndices; }
    [[nodiscard]]
    unsigned int GetNumVerts() const { return mNumVerts; }

private:
    unsigned int mNumVerts;
    unsigned int mNumIndices;
    unsigned int mVertexBuffer;
    unsigned int mIndexBuffer;
    unsigned int mVertexArray;
};
