//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#include "Oneiro/Core/Config.hpp"
#include "Oneiro/Core/Logger.hpp"


namespace oe::Core
{
	Config::Config(const std::string& file)
	{
		try
		{
			if (!std::filesystem::is_directory("Configs"))
				std::filesystem::create_directory("Configs");
			mIFile.open("Configs/" + file, std::ios::app);
			mOFile.open("Configs/" + file, std::ios::app);
			if (!mIFile.is_open() || !mOFile.is_open())
				log::get("log")->warn("Failed to open config " + file + " file!");
		}
		catch (const std::exception& ex)
		{
			throw std::runtime_error(ex.what());
		}
	}

	std::string Config::GetValue(const std::string& cfg)
	{
		std::string line;
		while (std::getline(mIFile, line))
		{
			if (line.size() >= cfg.size() && std::string(1, line[cfg.size()]) == "=")
			{
				mIFile.seekg(0);
				line.erase(0, cfg.size() + 1);
				return line;
			}
		}
		mIFile.seekg(0);
		return "None";
	}

	void Config::WriteData(const std::string& cfg, const std::string& value)
	{
		mOFile.seekp(0);
		mOFile << cfg << "=" << value << '\n';
	}
}
