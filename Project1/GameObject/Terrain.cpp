#include "Terrain.h"
#include "../Primatives/Mesh.h"
#include "../Primatives/Vertex.h"
#include "../Rendering/Shading/Manager.h"
#include "../Utils/ResourceLoader.h"
#include "../Utils/NoiseGeneration.h"
#include "../Primatives/Materials/MaterialBase.h"

bool Terrain::gottenShader = false;
unsigned Terrain::shaderMesh = 0;

unsigned Terrain::shaderHM = 0;
Terrain::Terrain() : resolution(), transform(), groundBuffer(), heightMap(0), useTexture(true), /*lowest(nullptr), middle(nullptr), highest(nullptr),*/ renderWireframe(false), material(nullptr)
{
	if (NOT gottenShader) {
		shaderHM = ResourceLoader::getShader("DeferredTerrainMap");
		shaderMesh = ResourceLoader::getShader("DeferredTerrainMesh");
	}
}

Terrain::Terrain(Int res) : Terrain()
{
	resolution = res + 1;
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
glm::vec3 Terrain::getNormal(glm::vec2 pos) const
{
	return glm::vec3(0, 1, 0);
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
	const float adder = 1.0f / static_cast<float>(resolution-1);
	for (float y = 0; y <= 1; y += adder) {
		for (float x = 0; x <= 1; x += adder) {
			glm::vec2 tex(x, y);
			// tex.x /= resolution;
			// tex.y /= resolution;
			glm::vec3 pos(tex.x, getHeight(tex), tex.y);
			pos -= 0.5f;
			Primative::Vertex vertex(pos, tex, getNormal(tex));
			mesh.verts.push_back(vertex);
		}
	}
	getIndices(mesh.indices, resolution);
	groundBuffer = Primative::Buffers::VertexBuffer(mesh, GL_TRIANGLES);
}


void Terrain::draw(float deltaTime)
{
	if (renderWireframe)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDisable(GL_CULL_FACE);
	Render::Shading::Manager::setActive(useTexture ? shaderHM : shaderMesh);
	Render::Shading::Manager::setValue("model", transform.getModel());

	int unit = 0;
	if (useTexture) {
		Render::Shading::Manager::setValue("hm", unit);
		glActiveTexture(GL_TEXTURE0 + unit++);
		glBindTexture(GL_TEXTURE_2D, heightMap);
	}
	if (material) {
		material->activateTextures(unit);
		unsigned u = unit;
		Render::Shading::Manager::setValue("material", material, u);
	}

	/*if (lowest) {
		lowest->update(deltaTime);
		Render::Shading::Manager::setValue("lowest", *lowest, unit);
		lowest->tryBindTexture(unit);
	}
	if (middle) {
		middle->update(deltaTime);
		Render::Shading::Manager::setValue("middle", *middle, unit);
		middle->tryBindTexture(unit);
	}
	if (highest) {
		highest->update(deltaTime);
		Render::Shading::Manager::setValue("highest", *highest, unit);
		highest->tryBindTexture(unit);
	}*/
	
	groundBuffer.render();
	glEnable(GL_CULL_FACE);
	if(renderWireframe)
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void Terrain::cleanUp()
{
	/*highest->cleanUp();
	middle->cleanUp();
	lowest->cleanUp();*/
	groundBuffer.cleanUp();
	noise.clear();
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

void Terrain::setMaterial(Materials::MaterialBase* material)
{
	this->material = material;
}

/*void Terrain::setLowestTexture(Materials::MatItemBase<glm::vec3>* low)
{
	lowest = low;
}

void Terrain::setMiddleTexture(Materials::MatItemBase<glm::vec3>* mid)
{
	middle = mid;
}

void Terrain::setHighestTexture(Materials::MatItemBase<glm::vec3>* high)
{
	highest = high;
}*/

void Terrain::setPosition(Vector3 pos)
{
	transform.Position = pos;
}

void Terrain::setScale(Vector3 scale)
{
	transform.Scale = scale;
}

Component::Transform& Terrain::getTransform()
{
	return transform;
}

float Terrain::getHeight(const glm::vec2& position, bool scaled) const
{
	glm::vec2 pos = position;
	if (NOT scaled) {
		pos -= glm::vec2(transform.Position.x, transform.Position.z);
		pos /= glm::vec2(transform.Scale.x, transform.Scale.z);	
		pos += 0.5f;
	}
	pos *= resolution;
	if (glm::any(glm::greaterThanEqual(pos, glm::vec2(resolution)) OR glm::lessThan(pos, glm::vec2(0)))) {
		return 0.5f;
	}
	if (NOT noise.size())
		return 0.5f;
	glm::ivec2 p = static_cast<glm::ivec2>(pos);
	if(scaled)
		return noise[p.x * resolution + p.y] + 0.5f;
	return (noise[p.x * resolution + p.y]) * transform.Scale.y;
}

float Terrain::getHeight(Float x, Float z, bool scaled) const
{
	return getHeight({ x, z }, scaled);
}

Vector3 Terrain::getPosition() const
{
	return transform.Position;
}

Vector3 Terrain::getScale() const
{
	return transform.Scale;
}

const Materials::MaterialBase* Terrain::getMaterial() const
{
	return material;
}
