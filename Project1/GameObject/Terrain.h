#pragma once
#include "../Utils/General.h"
#include "../Primatives/Buffers/VertexBuffer.h"
#include "../Componets/Componets.h"
#include "../Primatives/Material.h"

class Terrain
{
private:
	void init();
	Materials::PBR material;
	int resolution;
	Component::Transform transform;
	Primative::Buffers::VertexBuffer groundBuffer;
	std::vector<unsigned> seperators;
	unsigned heightMap;
	std::array<unsigned, 3> textures;
	static bool gottenShader;
	static unsigned shader;
public:
	~Terrain() = default;
	Terrain();
	Terrain(Int res);
	void draw();
	void cleanUp();
	void setHeightMap(unsigned tex);
	void setTextures(Unsigned tex1, Unsigned tex2, Unsigned tex3);

	Materials::PBR& getMaterial();

	Component::Transform& getTransform();
};

