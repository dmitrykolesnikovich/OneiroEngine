//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#include "Oneiro/Core/Random.hpp"

namespace oe::Core
{
	int Random::DiceInt(int min, int max)
	{
		std::uniform_int_distribution<int> uid(min, max);
		return uid(mGenerator);
	}

	float Random::DiceFloat(float min, float max)
	{
		std::uniform_real_distribution<float> uid(min, max);
		return uid(mGenerator);
	}

	double Random::DiceDouble(double min, double max)
	{
		std::uniform_real_distribution<double> uid(min, max);
		return uid(mGenerator);
	}

	std::mt19937 Random::mGenerator{};
}
