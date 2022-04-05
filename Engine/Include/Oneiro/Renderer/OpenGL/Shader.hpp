//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#pragma once

#include <string>
#include <unordered_map>
#include "OpenGL/gl_core_4_5.hpp"
#include "glm/vec3.hpp"
#include "glm/mat4x4.hpp"
#define OE_DLL_EXPORT
#include "Oneiro/Core/Oneiro.hpp"

namespace oe::Renderer
{
    class OE_API Shader
    {
    public:
        ~Shader();
        void LoadFromFile(const std::string& path);
        void LoadFromSource(const std::string& vSrc, const std::string& fSrc);

        void Use() const;

        void SetUniform(const char* uName, int uVal) const;
        void SetUniform(const char* uName, float uVal) const;
        void SetUniform(const char* uName, const glm::mat4& uVal) const;
        void SetUniform(const char* uName, const glm::vec2& uVal) const;
        void SetUniform(const char* uName, const glm::vec3& uVal) const;
    private:
        GLint GetUniformLocation(const char* name) const;
        mutable std::unordered_map<const char*, GLint> mUniformLocationCache;
        uint32_t mID{};
    };
}
