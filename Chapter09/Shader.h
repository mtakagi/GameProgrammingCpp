#pragma once

#include <GL/glew.h>
#include <string>
#include "Math.h"

class Shader {
public:
    Shader();
    ~Shader();

    bool Load(const std::string & vertName, const std::string & fragName);
    void Unload() const;

    void SetActive() const;
    void SetMatrixUniform(const char* name, const Matrix4& matrix) const;
    void SetVectorUniform(const char* name, const Vector3& vector) const;
    void SetFloatUniform(const char* name, float value) const;

private:
    bool CompileShader(const std::string & fileName, GLenum shaderType, GLuint& outShader) const;
    [[nodiscard]]
    bool IsCompiled(GLuint shader) const;
    [[nodiscard]]
    bool IsValidProgram() const;

    GLuint mVertexShader;
    GLuint mFragShader;
    GLuint mShaderProgram;
};
