//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#include "Oneiro/Core/Random.hpp"

#include <sstream>

namespace oe::Core
{
	int Random::DiceInt(int min, int max)
	{
		const std::uniform_int_distribution uid(min, max);
		return uid(mGenerator);
	}

	float Random::DiceFloat(float min, float max)
	{
		const std::uniform_real_distribution uid(min, max);
		return uid(mGenerator);
	}

	double Random::DiceDouble(double min, double max)
	{
		const std::uniform_real_distribution uid(min, max);
		return uid(mGenerator);
	}

	std::string Random::DiceUUID()
	{
        std::stringstream ss{};
        int i{};
        ss << std::hex;
        for (i = 0; i < 8; i++)
            ss << mDis(mGenerator);
        ss << "-";
        for (i = 0; i < 4; i++)
            ss << mDis(mGenerator);
        ss << "-4";
        for (i = 0; i < 3; i++)
            ss << mDis(mGenerator);
        ss << "-";
        ss << mDis2(mGenerator);
        for (i = 0; i < 3; i++)
            ss << mDis(mGenerator);
        ss << "-";
        for (i = 0; i < 12; i++)
            ss << mDis(mGenerator);
        return ss.str();
	}
}
