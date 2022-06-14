//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#include "Oneiro/Renderer/OpenGL/Shader.hpp"

namespace oe::Renderer::GL
{
	Shader::~Shader()
	{
		gl::DeleteProgram(mID);
	}

	void Shader::CreateProgram()
	{
		mID = gl::CreateProgram();
		for (auto shaderSource : mShaderSources)
			gl::AttachShader(mID, shaderSource);
		gl::LinkProgram(mID);
		CheckCompileError(mID, "PROGRAM");
		for (auto shaderSource : mShaderSources)
			gl::DeleteShader(shaderSource);
	}

	void Shader::Use() const { gl::UseProgram(mID); }

	void Shader::ClearUniformLocationCache()
	{
		mUniformLocationCache.clear();
	}

	std::string Shader::LoadShaderFile(const char* path)
	{
		std::ifstream file;
		std::stringstream stringstream;

		file.open(path);
		if (!file.is_open())
		{
			std::cerr << "Shader::LoadShaderFile from " << path << '\n';
			return {};
		}
		stringstream << file.rdbuf();
		file.close();
		return stringstream.str();
	}

	uint32_t Shader::Get() { return mID; }

	bool Shader::CheckCompileStatus(uint32_t id)
	{
		int status{};
		gl::GetShaderiv(id, gl::COMPILE_STATUS, &status);
		return status;
	}

	bool Shader::CheckCompileError(uint32_t id, const char* type)
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
		return true;
	}
}
