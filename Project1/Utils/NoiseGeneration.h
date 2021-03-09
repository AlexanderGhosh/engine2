#pragma once
#include "General.h"
namespace Utils{
	using NoiseMap = std::vector<float>;
	using NoiseMatrix = std::vector<std::vector<float>>;
	class NoiseGeneration
	{
	public:
		static NoiseMap getMap(Int resolution, const std::vector<float>& ratios, const std::vector<float>& frequencys);
		static NoiseMatrix getMatrix(Int resolution, const std::vector<float>& ratios, const std::vector<float>& frequencys);
	};
}
