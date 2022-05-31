//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#pragma once

#include <string>
#include <unordered_map>
#include <iostream>
#include <fstream>
#include <sstream>

#include "glm/vec3.hpp"
#include "glm/mat4x4.hpp"

#include "Base.hpp"

namespace oe::Renderer::GL
{
    class Shader
    {
    public:
        ~Shader();

        template<int ShaderType>
        constexpr void LoadShaderSrc(const char* src)
        {
            if (mID != 0)
                return;
            const uint32_t shader = gl::CreateShader(ShaderType);
            gl::ShaderSource(shader, 1, &src, nullptr);
            gl::CompileShader(shader);

            if (CheckCompileError(shader, "SHADER"))
                mShaderSources.push_back(shader);
            else
                gl::DeleteShader(shader);
        }

        template<int ShaderType>
        constexpr void LoadShaderSrc(const std::string& src)
        {
            LoadShaderSrc<ShaderType>(src.c_str());
        }

        void CreateProgram();

        void Use() const;

        IS_SAME_TEMPLATE(bool)
        constexpr void SetUniform(const char* name, T value) const
        {
            gl::Uniform1i(GetUniformLocation(name), value);
        }

        IS_SAME_TEMPLATE(char)
        constexpr void SetUniform(const char* name, T value) const
        {
            gl::Uniform1i(GetUniformLocation(name), value);
        }

        IS_SAME_TEMPLATE(unsigned int)
        constexpr void SetUniform(const char* name, T value) const
        {
            gl::Uniform1ui(GetUniformLocation(name), value);
        }

        IS_SAME_TEMPLATE(unsigned int)
        constexpr void SetUniform(const char* name, T first, T second) const
        {
            gl::Uniform2ui(GetUniformLocation(name), first, second);
        }

        IS_SAME_TEMPLATE(unsigned int)
        constexpr void SetUniform(const char* name, T first, T second, T third) const
        {
            gl::Uniform3ui(GetUniformLocation(name), first, second, third);
        }

        IS_SAME_TEMPLATE(unsigned int)
        constexpr void SetUniform(const char* name, T first, T second, T third, T fourth)
        const
        {
            gl::Uniform4ui(GetUniformLocation(name), first, second, third, fourth);
        }

        IS_SAME_TEMPLATE(int)
        constexpr void SetUniform(const char* name, T value) const
        {
            gl::Uniform1i(GetUniformLocation(name), value);
        }

        IS_SAME_TEMPLATE(int)
        constexpr void SetUniform(const char* name, T first, T second) const
        {
            gl::Uniform2i(GetUniformLocation(name), first, second);
        }

        IS_SAME_TEMPLATE(int)
        constexpr void SetUniform(const char* name, T first, T second, T third) const
        {
            gl::Uniform3i(GetUniformLocation(name), first, second, third);
        }

        IS_SAME_TEMPLATE(int)
        constexpr void SetUniform(const char* name, T first, T second, T third, T fourth) const
        {
            gl::Uniform4i(GetUniformLocation(name), first, second, third, fourth);
        }

        IS_SAME_TEMPLATE(float)
        constexpr void SetUniform(const char* name, T value) const
        {
            gl::Uniform1f(GetUniformLocation(name), value);
        }

        IS_SAME_TEMPLATE(float)
        constexpr void SetUniform(const char* name, T first, T second) const
        {
            gl::Uniform2f(GetUniformLocation(name), first, second);
        }

        IS_SAME_TEMPLATE(float)
        constexpr void SetUniform(const char* name, T first, T second, T third) const
        {
            gl::Uniform3f(GetUniformLocation(name), first, second, third);
        }

        IS_SAME_TEMPLATE(glm::mat4)
        constexpr void SetUniform(const char* name, T first, T second, T third, T
        fourth) const
        {
            gl::Uniform4f(GetUniformLocation(name), first, second, third, fourth);
        }

        IS_SAME_TEMPLATE(glm::vec3)
        constexpr void SetUniform(const char* name, const T& vec) const
        {
            gl::Uniform3fv(GetUniformLocation(name), 1, &vec[0]);
        }

        IS_SAME_TEMPLATE(glm::mat4)
        constexpr void SetUniform(const char* name, const T& mat) const
        {
            gl::UniformMatrix4fv(GetUniformLocation(name), 1, gl::FALSE_, &mat[0][0]);
        }

        [[maybe_unused]] void ClearUniformLocationCache();

        /// Load shader file and return source code of it.
        /// \param path path to shader
        /// \return source code of shader
        static std::string LoadShaderFile(const char* path);

        uint32_t Get();
    private:
        static bool CheckCompileStatus(uint32_t id);

        static bool CheckCompileError(uint32_t id, const char* type);

        template<class T = int>
        constexpr GLint GetUniformLocation(const char* name) const
        {
            if (mUniformLocationCache.find(name) != mUniformLocationCache.end())
                return mUniformLocationCache[name];
            const GLint location = gl::GetUniformLocation(mID, name);
            mUniformLocationCache[name] = location;
            return location;
        }

        mutable std::unordered_map<const char*, GLint> mUniformLocationCache;
        std::vector<uint32_t> mShaderSources{};
        uint32_t mID{};
    };
}
