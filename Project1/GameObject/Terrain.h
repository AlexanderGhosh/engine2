#pragma once
#include "../Utils/General.h"
#include "../Primatives/Buffers/VertexBuffer.h"
#include "../Componets/Componets.h"
namespace Utils {
	using NoiseMap = std::vector<float>;
}
class Terrain
{
private:
	void init();
	void getIndices(std::vector<unsigned>& indices, int resolution);
	float getHeight(glm::vec2 pos) const;
	glm::vec3 getNormal(glm::vec2 pos) const;
	int resolution;
	bool useTexture;
	Component::Transform transform;
	Primative::Buffers::VertexBuffer groundBuffer;
	Utils::NoiseMap noise;
	unsigned heightMap;
	static bool gottenShader;
	static unsigned shaderHM, shaderMesh;
public:
	~Terrain() = default;
	Terrain();
	Terrain(Int res);
	void draw();
	void cleanUp();
	void setHeightMap(unsigned tex);
	void setNoiseBuffer(const Utils::NoiseMap& noise);
	void useTextureMap(bool use);

	Component::Transform& getTransform();
};

