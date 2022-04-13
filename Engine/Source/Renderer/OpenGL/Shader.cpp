//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#include <fstream>
#include <sstream>
#include <cstring>
#include <iostream>
#include "Oneiro/Renderer/OpenGL/Shader.hpp"
#include "Oneiro/Core/Logger.hpp"

uint32_t CreateVertexShader(const char* src);
uint32_t CreateFragmentShader(const char* src);
uint32_t CreateProgram(uint32_t vID, uint32_t fID);
void CheckCompileErrors(uint32_t ID, const char* type);

enum ShaderType
{
    VERTEX,
    FRAGMENT
};

oe::Renderer::Shader::~Shader()
{
    gl::DeleteShader(mID);
}

void oe::Renderer::Shader::LoadFromFile(const std::string& path)
{
    std::string vertexShaderSrc{};
    std::string fragmentShaderSrc{};

    std::ifstream shaderFile{};

    try
    {
        shaderFile.open(path);

        std::stringstream ss[2];
        std::string line{};
        ShaderType type{};

        while (std::getline(shaderFile, line))
        {
            if (line.find("// TYPE=") != std::string::npos)
            {
                if (line.find("VERTEX") != std::string::npos)
                    type = VERTEX;
                else if (line.find("FRAGMENT") != std::string::npos)
                    type = FRAGMENT;
            }
            else
            {
                ss[static_cast<int>(type)] << line << '\n';
            }
        }

        shaderFile.close();

        vertexShaderSrc = ss[static_cast<int>(VERTEX)].str();
        fragmentShaderSrc = ss[static_cast<int>(FRAGMENT)].str();
    }
    catch (const std::exception& ex)
    {
        Logger::Get("log")->PrintWarning(
            "Failed to load shader at" + path + " path! Exception: " + ex.what());
    }

    const uint32_t vertexShader = CreateVertexShader(vertexShaderSrc.c_str());
    const uint32_t fragmentShader = CreateFragmentShader(fragmentShaderSrc.c_str());

    mID = CreateProgram(vertexShader, fragmentShader);

    gl::DeleteShader(vertexShader);
    gl::DeleteShader(fragmentShader);
}

void oe::Renderer::Shader::LoadFromSource(const std::string& vSrc, const std::string& fSrc)
{
	const uint32_t vShader = CreateVertexShader(vSrc.c_str());
    const uint32_t fShader = CreateFragmentShader(fSrc.c_str());
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
    const GLint location = gl::GetUniformLocation(mID, name);
    mUniformLocationCache[name] = location;
    return location;
}

uint32_t CreateVertexShader(const char* src)
{
	const uint32_t ID = gl::CreateShader(gl::VERTEX_SHADER);
    gl::ShaderSource(ID, 1, &src, nullptr);
    gl::CompileShader(ID);
    CheckCompileErrors(ID, "VERTEX");
    return ID;
}

uint32_t CreateFragmentShader(const char* src)
{
	const uint32_t ID = gl::CreateShader(gl::FRAGMENT_SHADER);
    gl::ShaderSource(ID, 1, &src, nullptr);
    gl::CompileShader(ID);
    CheckCompileErrors(ID, "FRAGMENT");
    return ID;
}

uint32_t CreateProgram(uint32_t vID, uint32_t fID)
{
	const uint32_t ID = gl::CreateProgram();
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