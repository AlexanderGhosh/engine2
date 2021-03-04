#pragma once
#include "../Utils/General.h"
#include "../Primatives/Buffers.h"
#include "../Componets/Componets.h"

class Terrain
{
private:
	void init();
	int resolution;
	Component::Transform transform;
	Primative::VertexBuffer groundBuffer;
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

	Component::Transform& getTransform();
};

