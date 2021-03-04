#include "Terrain.h"
#include "../Primatives/Mesh.h"
#include "../Primatives/Vertex.h"
#include "../Rendering/Shading/Manager.h"
#include "../Utils/ResourceLoader.h"

bool Terrain::gottenShader = false;
unsigned Terrain::shader = 0;

Terrain::Terrain() : resolution(), transform(), groundBuffer(), seperators(), textures()
{
	if (NOT gottenShader) {
		shader = ResourceLoader::getShader("TerrainShader");
	}
}

Terrain::Terrain(Int res) : Terrain()
{
	resolution = res;
	init();
}
void getIndices(std::vector<unsigned>& indices, int resolution) {

	for (int gz = 0; gz < resolution - 1; gz++) {
		for (int gx = 0; gx < resolution - 1; gx++) {
			unsigned int topLeft = (gz * resolution) + gx;
			unsigned int topRight = topLeft + 1;
			unsigned int bottomLeft = ((gz + 1) * resolution) + gx;
			unsigned int bottomRight = bottomLeft + 1;
			std::vector<unsigned int> ind = {
				topLeft,
				bottomLeft,
				topRight,
				topRight,
				bottomLeft,
				bottomRight
			};
			indices.insert(indices.end(), ind.begin(), ind.end());
		}
	}
}
void Terrain::init()
{
	Primative::Mesh mesh;
	for (int y = 0; y < resolution; y++) {
		for (int x = 0; x < resolution; x++) {
			glm::vec2 tex(x, y);
			tex.x /= resolution;
			tex.y /= resolution;
			glm::vec3 pos(tex.x, 0.5f, tex.y);
			pos -= 0.5f;
			Primative::Vertex vertex(pos, tex, Utils::yAxis());
			mesh.verts.push_back(vertex);
		}
	}
	getIndices(mesh.indices, resolution);
	groundBuffer = Primative::VertexBuffer(mesh, GL_TRIANGLES);
}


void Terrain::draw()
{
	glDisable(GL_CULL_FACE);
	Render::Shading::Manager::setActive(shader);
	Render::Shading::Manager::setValue("model", transform.getModel());

	glActiveTexture(GL_TEXTURE0);
	Render::Shading::Manager::setValue("hm", 0);
	glBindTexture(GL_TEXTURE_2D, heightMap);

	for (char i = 0; i < 3; i++) {
		glActiveTexture(GL_TEXTURE1 + i);
		Render::Shading::Manager::setValue("textures[" + std::to_string(i) + "]", i + 1);
		glBindTexture(GL_TEXTURE_2D, textures[i]);
	}

	groundBuffer.bind();
	groundBuffer.draw();
	groundBuffer.unBind();
	glEnable(GL_CULL_FACE);
}

void Terrain::cleanUp()
{
	groundBuffer.cleanUp();
}

void Terrain::setHeightMap(unsigned tex)
{
	heightMap = tex;
}

void Terrain::setTextures(Unsigned tex1, Unsigned tex2, Unsigned tex3)
{
	textures = {
		tex1, tex2, tex3
	};
}

Component::Transform& Terrain::getTransform()
{
	return transform;
}
