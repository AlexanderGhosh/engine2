#include "ParticleEmmiter.h"
#include "../Utils/ResourceLoader.h"
#include "../Rendering/Shading/Manager.h"
#include "../Primatives/Model.h"
#include "../GameObject/GameObject.h"

int Component::ParticleEmmiter::shader = 0;
Primative::Buffers::VertexBuffer Component::ParticleEmmiter::quadBuffer = {};

void Component::ParticleEmmiter::spawn()
{
	particleTransforms.reserve(maxParticelCount);
	particleTransforms.resize(maxParticelCount);
	life.reserve(maxParticelCount);
	life.resize(maxParticelCount);
	for (unsigned i = 0; i < maxParticelCount; i++) {
		Transform& transform = particleTransforms[i];
		transform.Position = orgiCenter;
		for (char j = 0; j < 3; j++) {
			transform.Position[j] += Utils::random(0, 2 * oridRadii[j]) - oridRadii[j];
			transform.Scale *= 0.25f;
		}
		this->life[i] = maxLife;
	}
}

Component::ParticleEmmiter::ParticleEmmiter() : ComponetBase(), particleTransforms(), life(), maxParticelCount(0), velocity(0, -1, 0), doLoop(false), oridRadii(), orgiCenter(), maxLife(), colour(1), textures()
{
	if (NOT shader) {
		shader = ResourceLoader::getShader("ParticleShader");
		quadBuffer = ResourceLoader::getBuffer(ResourceLoader::getModel("plane.dae").getBuffers()[0]);
	}
}

Component::ParticleEmmiter::ParticleEmmiter(Unsigned numberOfParticles, Unsigned life, Vector3 center, Vector3 radii) : ParticleEmmiter()
{
	maxLife = life;
	orgiCenter = center;
	oridRadii = radii;
	maxParticelCount = numberOfParticles % (MAX_PARTICLES + 1);

	spawn();
}

void Component::ParticleEmmiter::update(float deltaTime)
{
	if (doLoop AND isDead())
		spawn();
	for (unsigned i = 0; i < particleTransforms.size(); i++) {
		float& l = life[i];
		Component::Transform& transform = particleTransforms[i];
		//l -= deltaTime;
		if (l <= 0) {
			Utils::removeAt(life, i);
			Utils::removeAt(particleTransforms, i);
			i--;
		}
		else {
			//transform.Position += velocity * deltaTime;
		}
	}
	textures.update(deltaTime);
	drawParticles();
}

void Component::ParticleEmmiter::drawParticles()
{
	Render::Shading::Manager::setActive(shader);
	for (int i = 0; i < particleTransforms.size(); i++) {
		glm::quat rot = particleTransforms[i].Rotation;

		Render::Shading::Manager::setValue("center[" + std::to_string(i) + "]", particleTransforms[i].Position);
		Render::Shading::Manager::setValue("scale[" + std::to_string(i) + "]", particleTransforms[i].Scale);
		particleTransforms[i].Rotation = rot;
	}
	Render::Shading::Manager::setValue("col", colour);
	Render::Shading::Manager::setValue("mix_val", (textures.hasTextures() ? 0.0f : 1.0f));
	Render::Shading::Manager::setValue("tex", 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textures.getTexture());
	quadBuffer.bind();
	glDrawElementsInstanced(quadBuffer.getDrawType(), quadBuffer.getIndicesCount(), GL_UNSIGNED_INT, 0, particleTransforms.size());
	quadBuffer.unBind();
}

void Component::ParticleEmmiter::cleanUp()
{
	particleTransforms.clear();
	life.clear();
}

void Component::ParticleEmmiter::restart()
{
	spawn();
}

void Component::ParticleEmmiter::loop()
{
	doLoop = true;
}

bool Component::ParticleEmmiter::isDead() const
{
	return NOT life.size();
}

void Component::ParticleEmmiter::setCenter(Vector3 center)
{
	return;
	for (Component::Transform& trans : particleTransforms) {
		trans.Position -= orgiCenter;
		trans.Position += center;
	}
	orgiCenter = center;
}

void Component::ParticleEmmiter::setTexture(const Primative::TextureChain& tex)
{
	this->textures = tex;
}

void Component::ParticleEmmiter::setColour(Vector4 col)
{
	colour = col;
}
