//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#pragma once

#include <random>

namespace oe::Core
{
	class Random
	{
	public:
		static int DiceInt(int min, int max);
		static float DiceFloat(float min, float max);
		static double DiceDouble(double min, double max);
	private:
		static std::mt19937 mGenerator;
	};
}