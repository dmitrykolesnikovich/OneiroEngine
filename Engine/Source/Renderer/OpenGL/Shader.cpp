//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#include <fstream>
#include <cstring>
#include <iostream>
#include "Oneiro/Renderer/OpenGL/Shader.hpp"
#include "Oneiro/Core/Logger.hpp"
#include "Oneiro/Core/Root.hpp"

namespace
{
    uint32_t createVertexShader(const char* shaderSource);
    uint32_t createFragmentShader(const char* shaderSource);
    uint32_t createProgram(GLuint vertexShaderId, GLuint fragmentShaderId);
    void checkCompileErrors(GLuint id, const char* type);

    enum ShaderType
    {
        VERTEX,
        FRAGMENT
    };
}

namespace oe::Renderer
{
    Shader::~Shader()
    {
        gl::DeleteShader(mID);
    }

    void Shader::LoadFromFile(const std::string& path)
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
                if (line.find("// VERTEX SHADER") != std::string::npos)
                {
                    type = VERTEX;
                    ss[static_cast<int>(type)] << "#version 330 core" << '\n';
                }
                else if (line.find("// FRAGMENT SHADER") != std::string::npos)
                {
                    type = FRAGMENT;
                    ss[static_cast<int>(type)] << "#version 330 core" << '\n';
                    ss[static_cast<int>(type)] << "out vec4 FragColor;" << '\n';
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
            log::get("log")->warn("Failed to load shader at" + path + " path! Exception: " + ex.what());
        }

        const uint32_t vertexShader = createVertexShader(vertexShaderSrc.c_str());
        const uint32_t fragmentShader = createFragmentShader(fragmentShaderSrc.c_str());

        mID = createProgram(vertexShader, fragmentShader);

        gl::DeleteShader(vertexShader);
        gl::DeleteShader(fragmentShader);
    }

    void Shader::LoadFromSource(const std::string& vSrc, const std::string& fSrc)
    {
        const uint32_t vShader = createVertexShader(vSrc.c_str());
        const uint32_t fShader = createFragmentShader(fSrc.c_str());
        mID = createProgram(vShader, fShader);
        gl::DeleteShader(vShader);
        gl::DeleteShader(fShader);
    }

    void Shader::Use() const
    {
        gl::UseProgram(mID);
    }

    void Shader::SetUniform(const char* uName, int uVal) const
    {
        gl::Uniform1i(GetUniformLocation(uName), uVal);
    }

    void Shader::SetUniform(const char* uName, float uVal) const
    {
        gl::Uniform1f(GetUniformLocation(uName), uVal);
    }

    void Shader::SetUniform(const char* uName, const glm::vec2& uVal) const
    {
        gl::Uniform2fv(GetUniformLocation(uName), 1, &uVal[0]);
    }

    void Shader::SetUniform(const char* uName, const glm::vec3& uVal) const
    {
        gl::Uniform3fv(GetUniformLocation(uName), 1, &uVal[0]);
    }

    void Shader::SetUniform(const char* uName, const glm::mat4& uVal) const
    {
        gl::UniformMatrix4fv(GetUniformLocation(uName), 1, gl::FALSE_, &uVal[0][0]);
    }
}

GLint oe::Renderer::Shader::GetUniformLocation(const char* name) const
{
    if (mUniformLocationCache.find(name) != mUniformLocationCache.end())
        return mUniformLocationCache[name];
    const GLint location = gl::GetUniformLocation(mID, name);
    mUniformLocationCache[name] = location;
    return location;
}

namespace
{
    uint32_t createVertexShader(const char* shaderSource)
    {
        const auto id = gl::CreateShader(gl::VERTEX_SHADER);
        gl::ShaderSource(id, 1, &shaderSource, nullptr);
        gl::CompileShader(id);
        checkCompileErrors(id, "VERTEX");
        return id;
    }

    uint32_t createFragmentShader(const char* shaderSource)
    {
        const auto id = gl::CreateShader(gl::FRAGMENT_SHADER);
        gl::ShaderSource(id, 1, &shaderSource, nullptr);
        gl::CompileShader(id);
        checkCompileErrors(id, "FRAGMENT");
        return id;
    }

    uint32_t createProgram(GLuint vertexShaderId, GLuint fragmentShaderId)
    {
        const auto id = gl::CreateProgram();
        gl::AttachShader(id, vertexShaderId);
        gl::AttachShader(id, fragmentShaderId);
        gl::LinkProgram(id);
        checkCompileErrors(id, "PROGRAM");
        return id;
    }

    void checkCompileErrors(GLuint id, const char* type)
    {
        int success{};
        char infoLog[512];

        if (std::strcmp(type, "PROGRAM") != 0)
        {
            gl::GetShaderiv(id, gl::COMPILE_STATUS, &success);
            if (!success)
            {
                gl::GetShaderInfoLog(id, 512, nullptr, infoLog);
                std::cerr << infoLog << '\n';
            }
        }
        else
        {
            gl::GetProgramiv(id, gl::LINK_STATUS, &success);
            if (!success)
            {
                gl::GetProgramInfoLog(id, 512, nullptr, infoLog);
                std::cerr << infoLog << '\n';
            }
        }
    }
}