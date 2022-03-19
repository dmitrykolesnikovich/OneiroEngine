//
// Created by Dezlow on 11.02.2022.
// Copyright (c) 2022 Oneiro Games. All rights reserved.
//

#pragma once

#ifndef ONEIRO_SHADER_HPP
#define ONEIRO_SHADER_HPP

#include <unordered_map>
#include "OpenGL/gl_core_4_5.hpp"
#include "glm/vec3.hpp"
#include "glm/mat4x4.hpp"
#define OE_DLL_EXPORT
#include "Oneiro/Core/Oneiro.hpp"

namespace oe
{
    class OE_API Shader
    {
    public:
        ~Shader();
        void LoadFromFile(const std::string& path);
        void LoadFromSource(const std::string& vSrc, const std::string& fSrc);
        void Use() const;

        void SetUniform(const char* uName, int uVal);
        void SetUniform(const char* uName, float uVal);
        void SetUniform(const char* uName, const glm::mat4& uVal);
        void SetUniform(const char* uName, const glm::vec3& uVal);
    private:
        GLint GetUniformLocation(const char* name);
        mutable std::unordered_map<const char*, GLint> mUniformLocationCache;
        uint32_t mID{};
    };
}

#endif //ONEIRO_SHADER_HPP
