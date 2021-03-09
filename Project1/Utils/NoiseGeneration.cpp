#include "NoiseGeneration.h"
#include <gtc/noise.hpp>

Utils::NoiseMap Utils::NoiseGeneration::getMap(Int resolution, const std::vector<float>& ratios, const std::vector<float>& frequencys)
{
	NoiseMap res;
	res.reserve(resolution * resolution);
	res.resize(resolution * resolution);
	for (int x = 0; x < resolution; x++) {
		for (int y = 0; y < resolution; y++) {
			float noise = 0;
			float e = 0;
			for (int l = 0; l < ratios.size(); l++) {
				noise += ratios[l] * glm::perlin(glm::vec2(x, y) / static_cast<float>(resolution) * frequencys[l]);
				e += ratios[l];
			}
			noise /= e;
			res[x * resolution + y] = noise;
		}
	}
	return res;
}

Utils::NoiseMatrix Utils::NoiseGeneration::getMatrix(Int resolution, const std::vector<float>& ratios, const std::vector<float>& frequencys)
{
	NoiseMatrix res;
	res.reserve(resolution);
	res.resize(resolution);
	for (int x = 0; x < resolution; x++) {
		res[x].reserve(resolution);
		res[x].resize(resolution);
		for (int y = 0; y < resolution; y++) {
			float noise = 0;
			float e = 0;
			for (int l = 0; l < ratios.size(); l++) {
				noise += ratios[l] * glm::perlin(glm::vec2(x, y) / static_cast<float>(resolution) * frequencys[l]);
				e += ratios[l];
			}
			noise /= e;
			res[x][y] = noise;
		}
	}
	return res;
}
