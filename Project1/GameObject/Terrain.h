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
	class MaterialBase;
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
	Materials::MaterialBase* material;
	/*Materials::MatItemBase<glm::vec3>* lowest;
	Materials::MatItemBase<glm::vec3>* middle;
	Materials::MatItemBase<glm::vec3>* highest;*/
public:
	~Terrain() = default;
	Terrain();
	Terrain(Int res);
	void render(float deltaTime, bool bindShader);
	void cleanUp();
	void setHeightMap(unsigned tex);
	void setNoiseBuffer(const Utils::NoiseMap& noise);
	void useTextureMap(bool use);
	bool renderWireframe;

	// setters
	/*void setLowestTexture(Materials::MatItemBase<glm::vec3>* low);
	void setMiddleTexture(Materials::MatItemBase<glm::vec3>* mid);
	void setHighestTexture(Materials::MatItemBase<glm::vec3>* high);*/
	void setPosition(Vector3 pos);
	void setScale(Vector3 scale);
	void setMaterial(Materials::MaterialBase* material);

	// getters
	Component::Transform& getTransform();
	float getHeight(const glm::vec2& position, bool scaled = true) const;
	float getHeight(Float x, Float z, bool scaled = true) const;
	Vector3 getPosition() const;
	Vector3 getScale() const;
	const Materials::MaterialBase* getMaterial() const;
};

