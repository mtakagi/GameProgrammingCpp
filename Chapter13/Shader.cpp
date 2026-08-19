#include <fstream>
#include <sstream>
#include <SDL.h>
#include "Shader.h"

Shader::Shader()
    : mVertexShader(0)
    , mFragShader(0)
    , mShaderProgram(0) {
}

Shader::~Shader() = default;

bool Shader::Load(const std::string &vertName, const std::string &fragName) {
    if (!CompileShader(vertName, GL_VERTEX_SHADER, mVertexShader) ||
        !CompileShader(fragName, GL_FRAGMENT_SHADER, mFragShader)) {
        return false;
    }

    mShaderProgram = glCreateProgram();

    glAttachShader(mShaderProgram, mVertexShader);
    glAttachShader(mShaderProgram, mFragShader);
    glLinkProgram(mShaderProgram);

    if (!IsValidProgram()) {
        return false;
    }

    return true;
}

void Shader::Unload() const {
    glDeleteProgram(mShaderProgram);
    glDeleteShader(mVertexShader);
    glDeleteShader(mFragShader);
}

void Shader::SetActive() const {
    glUseProgram(mShaderProgram);
}

void Shader::SetMatrixUniform(const char* name, const Matrix4& matrix) const {
    const auto loc = glGetUniformLocation(mShaderProgram, name);
    glUniformMatrix4fv(loc, 1, GL_TRUE, matrix.GetAsFloatPtr());
}

void Shader::SetMatrixUniforms(const char *name, const Matrix4 *matrices, const unsigned count) const {
    auto loc = glGetUniformLocation(mShaderProgram, name);
    glUniformMatrix4fv(loc, static_cast<GLsizei>(count), GL_TRUE, matrices->GetAsFloatPtr());
}

void Shader::SetVectorUniform(const char *name, const Vector3 &vector) const {
    const auto loc = glGetUniformLocation(mShaderProgram, name);
    glUniform3fv(loc, 1, vector.GetAsFloatPtr());
}

void Shader::SetFloatUniform(const char *name, const float value) const {
    const auto loc = glGetUniformLocation(mShaderProgram, name);
    glUniform1f(loc, value);
}

void Shader::SetIntUniform(const char *name, const int value) const {
    const auto loc = glGetUniformLocation(mShaderProgram, name);
    glUniform1i(loc, value);
}

bool Shader::CompileShader(const std::string &fileName, const GLenum shaderType, GLuint &outShader) const {
    const std::ifstream shaderFile(fileName);

    if (shaderFile.is_open()) {
        std::stringstream sstream;
        sstream << shaderFile.rdbuf();
        const auto contents = sstream.str();
        const auto contentChar = contents.c_str();

        outShader = glCreateShader(shaderType);
        glShaderSource(outShader, 1, &contentChar, nullptr);
        glCompileShader(outShader);

        if (!IsCompiled(outShader)) {
            SDL_Log("シェーダー %s のコンパイルに失敗しました", fileName.c_str());

            return false;
        }
    } else {
        SDL_Log("シェーダーファイル %s が見つかりません", fileName.c_str());

        return false;
    }

    return true;
}

bool Shader::IsCompiled(const GLuint shader) const {
    GLint status;

    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

    if (status != GL_TRUE) {
        char buffer[512] = {};
        glGetShaderInfoLog(shader, 511, nullptr, buffer);
        SDL_Log("GLSL のコンパイルが失敗しました :\n%s", buffer);

        return false;
    }

    return true;
}

bool Shader::IsValidProgram() const {
    GLint status;

    glGetProgramiv(mShaderProgram, GL_LINK_STATUS, &status);

    if (status != GL_TRUE) {
        char buffer[512] = {};
        glGetProgramInfoLog(mShaderProgram, 511, nullptr, buffer);
        SDL_Log("GLSL Link Status:\n%s", buffer);

        return false;
    }

    return true;
}