//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#include "Oneiro/Renderer/OpenGL/Base.hpp"
#include "Oneiro/Renderer/OpenGL/ErrorHandler.hpp"
#include "Oneiro/Core/Logger.hpp"

namespace oe::Renderer::GL
{
	void ErrorHandler::GLClearError()
	{
		while (gl::GetError() != gl::NO_ERROR_);
	}

	bool ErrorHandler::GLLogCall()
	{
		while (GLenum error = gl::GetError())
		{
			log::get("log")->error("[OpenGL]: " + std::to_string(error));
			return false;
		}
		return true;
	}
}
