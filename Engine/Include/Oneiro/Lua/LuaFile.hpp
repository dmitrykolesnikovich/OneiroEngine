//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#pragma once

#include <fstream>
#include "sol/state.hpp"

namespace oe::Lua
{
	class File
	{
	public:
		template <class... Args>
		void OpenLibraries(Args&& ... args)
		{
			mState.open_libraries(std::forward<Args>(args)...);
		}

		sol::protected_function_result LoadFile(const std::string& pathToFile, bool out = true);

        [[nodiscard]] bool CallFunction(const std::string& name) const;
        [[nodiscard]] bool CallFunction(const std::string& name, const std::string& tableName) const;

        std::fstream& GetFile();
        sol::state* GetState();
    private:
		sol::state mState{};
		std::fstream mFile{};
	};
}
