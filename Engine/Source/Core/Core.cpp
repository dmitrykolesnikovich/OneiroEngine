//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#include <stdexcept>
#include "Oneiro/Core/Core.hpp"
#include "Oneiro/Core/Event.hpp"

#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"

#include "spdlog/spdlog.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"

#include "Oneiro/Core/Logger.hpp"

namespace oe::Core
{
	void Init()
	{
		glfwSetErrorCallback([](int error, const char* description)
		{
			Event::Dispatcher::Post(Event::ErrorEvent(error, description));
		});

		std::vector<log::sink_ptr> sinks;
		sinks.push_back(std::make_shared<log::sinks::basic_file_sink_mt>("log.txt", true));
		sinks.push_back(std::make_shared<log::sinks::stdout_color_sink_mt>());

		const auto firstLogger = std::make_shared<log::logger>("log", begin(sinks), end(sinks));
		const auto secondLogger = std::make_shared<log::logger>("dbg",
		                                                        std::make_shared<log::sinks::stdout_color_sink_mt>());
		firstLogger->set_pattern("[%H:%M:%S] [%t] [%^%l%$] %v");
		secondLogger->set_pattern("[%H:%M:%S] [%t] [%^%l in %s:%#%$] %v");
		set_default_logger(secondLogger);
		register_logger(firstLogger);

		if (!glfwInit())
			throw std::runtime_error("Failed to init glfw!");
	}

	void Shutdown() { glfwTerminate(); }
}
