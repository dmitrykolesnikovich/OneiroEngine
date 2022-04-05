//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#include <cstring>
#include <iostream>
#include "Oneiro/Renderer/OpenGL/Shader.hpp"

uint32_t CreateVertexShader(const char* src);
uint32_t CreateFragmentShader(const char* src);
uint32_t CreateProgram(uint32_t vID, uint32_t fID);
void CheckCompileErrors(uint32_t ID, const char* type);

oe::Renderer::Shader::~Shader()
{
    gl::DeleteShader(mID);
}

void oe::Renderer::Shader::LoadFromFile(const std::string& path)
{

}

void oe::Renderer::Shader::LoadFromSource(const std::string& vSrc, const std::string& fSrc)
{
    uint32_t vShader = CreateVertexShader(vSrc.c_str());
    uint32_t fShader = CreateFragmentShader(fSrc.c_str());
    mID = CreateProgram(vShader, fShader);
    gl::DeleteShader(vShader);
    gl::DeleteShader(fShader);
}

void oe::Renderer::Shader::Use() const
{
    gl::UseProgram(mID);
}

void oe::Renderer::Shader::SetUniform(const char* uName, int uVal) const
{
    gl::Uniform1i(GetUniformLocation(uName), uVal);
}

void oe::Renderer::Shader::SetUniform(const char* uName, float uVal) const
{
    gl::Uniform1f(GetUniformLocation(uName), uVal);
}

void oe::Renderer::Shader::SetUniform(const char* uName, const glm::vec2& uVal) const
{
    gl::Uniform2fv(GetUniformLocation(uName), 1, &uVal[0]);
}

void oe::Renderer::Shader::SetUniform(const char* uName, const glm::vec3& uVal) const
{
    gl::Uniform3fv(GetUniformLocation(uName), 1, &uVal[0]);
}

void oe::Renderer::Shader::SetUniform(const char* uName, const glm::mat4& uVal) const
{
    gl::UniformMatrix4fv(GetUniformLocation(uName), 1, gl::FALSE_, &uVal[0][0]);
}

GLint oe::Renderer::Shader::GetUniformLocation(const char* name) const
{
    if (mUniformLocationCache.find(name) != mUniformLocationCache.end())
        return mUniformLocationCache[name];
    GLint location = gl::GetUniformLocation(mID, name);
    mUniformLocationCache[name] = location;
    return location;
}

uint32_t CreateVertexShader(const char* src)
{
    uint32_t ID = gl::CreateShader(gl::VERTEX_SHADER);
    gl::ShaderSource(ID, 1, &src, nullptr);
    gl::CompileShader(ID);
    CheckCompileErrors(ID, "VERTEX");
    return ID;
}

uint32_t CreateFragmentShader(const char* src)
{
    uint32_t ID = gl::CreateShader(gl::FRAGMENT_SHADER);
    gl::ShaderSource(ID, 1, &src, nullptr);
    gl::CompileShader(ID);
    CheckCompileErrors(ID, "FRAGMENT");
    return ID;
}

uint32_t CreateProgram(uint32_t vID, uint32_t fID)
{
    uint32_t ID = gl::CreateProgram();
    gl::AttachShader(ID, vID);
    gl::AttachShader(ID, fID);
    gl::LinkProgram(ID);
    CheckCompileErrors(ID, "PROGRAM");
    return ID;
}

void CheckCompileErrors(uint32_t ID, const char* type)
{
    int success{};
    char infoLog[512];

    if (std::strcmp(type, "PROGRAM") != 0)
    {
        gl::GetShaderiv(ID, gl::COMPILE_STATUS, &success);
        if (!success)
        {
            gl::GetShaderInfoLog(ID, 512, nullptr, infoLog);
            std::cerr << infoLog << '\n';
        }
    }
    else
    {
        gl::GetProgramiv(ID, gl::LINK_STATUS, &success);
        if (!success)
        {
            gl::GetProgramInfoLog(ID, 512, nullptr, infoLog);
            std::cerr << infoLog << '\n';
        }
    }
}