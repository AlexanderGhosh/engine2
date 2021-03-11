#include "Terrain.h"
#include "../Primatives/Mesh.h"
#include "../Primatives/Vertex.h"
#include "../Rendering/Shading/Manager.h"
#include "../Utils/ResourceLoader.h"
#include "../Utils/NoiseGeneration.h"

bool Terrain::gottenShader = false;
unsigned Terrain::shaderMesh = 0;

unsigned Terrain::shaderHM = 0;
Terrain::Terrain() : resolution(), transform(), groundBuffer(), heightMap(0), useTexture(true)
{
	if (NOT gottenShader) {
		shaderHM = ResourceLoader::getShader("TerrainShaderHeightMap");
		shaderMesh = ResourceLoader::getShader("TerrainShaderMesh");
	}
}

Terrain::Terrain(Int res) : Terrain()
{
	resolution = res;
	init(); // creates the plane
}
void Terrain::getIndices(std::vector<unsigned>& indices, int resolution) {

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
float Terrain::getHeight(glm::vec2 pos) const
{
	pos *= resolution;
	if (glm::any(glm::greaterThan(pos, glm::vec2(resolution - 1)) OR glm::lessThan(pos, glm::vec2(0)))) {
		return 0.5f;
	}
	if(NOT noise.size())
		return 0.5f;
	glm::ivec2 p = static_cast<glm::ivec2>(pos);
	return noise[p.x * resolution + p.y] + 0.5f;
}
glm::vec3 Terrain::getNormal(glm::vec2 pos) const
{
	pos *= resolution;
	glm::vec3 off(1.0, 1.0, 0.0);
	glm::vec2 off1(1, 0);
	glm::vec2 off2(0, 1);
	float hL = getHeight(pos - off1);
	float hR = getHeight(pos + off1);
	float hD = getHeight(pos - off2);
	float hU = getHeight(pos + off2);

	// deduce terrain normal
	glm::vec3 norm(hL - hR, hD - hU, 2);
	norm.x = hL - hR;
	norm.y = hD - hU;
	norm.z = 2.0;
	return Utils::normalize(norm);
}
void Terrain::init()
{
	groundBuffer.cleanUp();
	Primative::Mesh mesh;
	for (int y = 0; y < resolution; y++) {
		for (int x = 0; x < resolution; x++) {
			glm::vec2 tex(x, y);
			tex.x /= resolution;
			tex.y /= resolution;
			glm::vec3 pos(tex.x, getHeight(tex), tex.y);
			pos -= 0.5f;
			Primative::Vertex vertex(pos, tex, getNormal(tex));
			mesh.verts.push_back(vertex);
		}
	}
	getIndices(mesh.indices, resolution);
	groundBuffer = Primative::Buffers::VertexBuffer(mesh, GL_TRIANGLES);
}


void Terrain::draw()
{
	// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDisable(GL_CULL_FACE);
	Render::Shading::Manager::setActive(useTexture ? shaderHM : shaderMesh);
	Render::Shading::Manager::setValue("model", transform.getModel());
	
	if (useTexture) {
		Render::Shading::Manager::setValue("hm", 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, heightMap);
	}
	
	groundBuffer.render();
	glEnable(GL_CULL_FACE);
	// glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void Terrain::cleanUp()
{
	groundBuffer.cleanUp();
}

void Terrain::setHeightMap(unsigned tex)
{
	heightMap = tex;
	useTexture = true;
}

void Terrain::setNoiseBuffer(const Utils::NoiseMap& noise)
{
	useTexture = false;
	this->noise = noise;
	init();
}

void Terrain::useTextureMap(bool use)
{
	useTexture = use;
}

Component::Transform& Terrain::getTransform()
{
	return transform;
}
