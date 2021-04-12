#pragma once
#include "../Utils/General.h"
#include "../Primatives/Buffers/VertexBuffer.h"
#include "../Componets/Componets.h"
namespace Utils {
	using NoiseMap = std::vector<float>;
}
namespace Materials {
	template<class T>
	class MatItemBase;
}
class Terrain
{
private:
	void init();
	void getIndices(std::vector<unsigned>& indices, int resolution);
	glm::vec3 getNormal(glm::vec2 pos) const;
	int resolution;
	Component::Transform transform;
	Primative::Buffers::VertexBuffer groundBuffer;
	Utils::NoiseMap noise;
	unsigned heightMap;
	bool useTexture;

	static bool gottenShader;
	static unsigned shaderHM, shaderMesh;

	Materials::MatItemBase<glm::vec3>* lowest;
	Materials::MatItemBase<glm::vec3>* middle;
	Materials::MatItemBase<glm::vec3>* highest;
public:
	~Terrain() = default;
	Terrain();
	Terrain(Int res);
	void draw(float deltaTime);
	void cleanUp();
	void setHeightMap(unsigned tex);
	void setNoiseBuffer(const Utils::NoiseMap& noise);
	void useTextureMap(bool use);

	// setters
	void setLowestTexture(Materials::MatItemBase<glm::vec3>* low);
	void setMiddleTexture(Materials::MatItemBase<glm::vec3>* mid);
	void setHighestTexture(Materials::MatItemBase<glm::vec3>* high);

	// getters
	Component::Transform& getTransform();
	float getHeight(const glm::vec2& position, bool scaled = true) const;
	float getHeight(Float x, Float z, bool scaled = true) const;
};

