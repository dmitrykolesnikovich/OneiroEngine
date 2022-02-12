//
// Created by Dezlow on 11.02.2022.
// Copyright (c) 2022 Oneiro Games. All rights reserved.
//

#pragma once

#ifndef ENGINE_SHADER_HPP
#define ENGINE_SHADER_HPP

#include <unordered_map>
#include "OpenGL/gl_core_4_5.hpp"
#include "DZL/string.hpp"
#include "glm/vec3.hpp"
#include "glm/mat4x4.hpp"

namespace oe
{
    namespace Renderer
    {
        class Shader
        {
        public:
            ~Shader();
            void LoadFromFile(const dzl::string& path);
            void LoadFromSource(const dzl::string& vSrc, const dzl::string& fSrc);
            void Use() const;

            void SetUniform(const char* uName, int uVal);
            void SetUniform(const char* uName, float uVal);
            void SetUniform(const char* uName, const glm::mat4& uVal);
            void SetUniform(const char* uName, const glm::vec3& uVal);
        private:
            GLint GetUniformLocation(const char* name);
            mutable std::unordered_map<const char*, GLint> mUniformLocationCache;
            dzl::uint32_t mID{};
        };
    }
}

#endif //ENGINE_SHADER_HPP
