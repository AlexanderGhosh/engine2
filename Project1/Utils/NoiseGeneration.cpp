#include "NoiseGeneration.h"
#include <gtc/noise.hpp>

Utils::NoiseMap Utils::NoiseGeneration::getMap(Vector3 position, Int resolution, const std::vector<float>& ratios, const std::vector<float>& frequencys)
{
	NoiseMap res;
	long res2 = static_cast<long>(resolution) * static_cast<long>(resolution);
	res.reserve(res2);
	float e = 0;
	for (char i = 0; i < ratios.size(); i++) {
		e += ratios[i];
	}
	float small = INFINITY;
	float large = -INFINITY;
	const float adder = 1.0f / static_cast<float>(resolution - 1);
	for (float x = 0; x <= 1; x += adder) {
		for (float y = 0; y <= 1; y += adder) {
			float noise = 0;
			for (int l = 0; l < ratios.size(); l++) {
				noise += ratios[l] * glm::perlin((position + glm::vec3(x, y, SEED)) * frequencys[l]);
				e += ratios[l];
			}
			//noise /= e;
			res.push_back(noise);
			if (noise > large) {
				large = noise;
			}
			else if (noise < small) {
				small = noise;
			}
		}
	}

	for (long i = 0; i < res2; i++) {
		//res[i] = Utils::map(res[i], { small, large }, { -0.5, 0.5 });
	}
	return res;
}

Utils::NoiseMatrix Utils::NoiseGeneration::getMatrix(Vector3 position, Int resolution, const std::vector<float>& ratios, const std::vector<float>& frequencys)
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
				noise += ratios[l] * glm::perlin((position + glm::vec3(x, y, SEED)) / static_cast<float>(resolution) * frequencys[l]);
				e += ratios[l];
			}
			noise /= e;
			res[x][y] = noise;
		}
	}
	return res;
}
