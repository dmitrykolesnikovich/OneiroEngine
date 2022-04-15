//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#pragma once

#include <fstream>
#include <string>
#include <filesystem>

namespace oe::Core
{
	class Config
	{
	public:
		Config(const std::string& file);
		std::string GetValue(const std::string& cfg);
		void WriteData(const std::string& cfg, const std::string& value);
	private:
		std::ifstream mIFile;
		std::ofstream mOFile;
	};
}
