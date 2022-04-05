#pragma once

#include <random>

namespace oe::Core
{
	class Random
	{
	public:
		static int DiceInt(int min, int max)
		{
			const std::uniform_int_distribution<int> uid(min, max);
			return uid(mGenerator);
		}

		static float DiceFloat(float min, float max)
		{
			const std::uniform_real_distribution<float> uid(min, max);
			return uid(mGenerator);
		}

		static double DiceDouble(double min, double max)
		{
			const std::uniform_real_distribution<double> uid(min, max);
			return uid(mGenerator);
		}
	private:
		static std::mt19937 mGenerator;
	};
}