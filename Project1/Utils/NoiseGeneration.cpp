#include "NoiseGeneration.h"
#include <gtc/noise.hpp>

Utils::NoiseMap Utils::NoiseGeneration::getMap(Int resolution, const std::vector<float>& ratios, const std::vector<float>& frequencys)
{
	NoiseMap res;
	long res2 = static_cast<long>(resolution) * static_cast<long>(resolution);
	res.reserve(res2);
	res.resize(res2);
	float e = 0;
	for (char i = 0; i < ratios.size(); i++) {
		e += ratios[i];
	}
	float small = INFINITY;
	float large = -INFINITY;
	for (int x = 0; x < resolution; x++) {
		for (int y = 0; y < resolution; y++) {
			float noise = 0;
			for (int l = 0; l < ratios.size(); l++) {
				noise += ratios[l] * glm::perlin((glm::vec2(x, y) / static_cast<float>(resolution)) * frequencys[l]);
			}
			noise /= e;
			res[x * resolution + y] = noise;
			if (noise > large) {
				large = noise;
			}
			else if (noise < small) {
				small = noise;
			}
		}
	}

	for (long i = 0; i < res2; i++) {
		res[i] = Utils::map(res[i], { small, large }, { -0.5, 0.5 });
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
