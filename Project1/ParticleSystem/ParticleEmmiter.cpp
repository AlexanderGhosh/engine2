#include "ParticleEmmiter.h"
#include <algorithm>
#include "../Utils/ResourceLoader.h"
#include "../Rendering/Shading/Manager.h"
#include "../Primatives/Model.h"
#include "../GameObject/GameObject.h"
#include "../Scene/GameScene.h"
#include "../Componets/Camera.h"
#include "Distributions/DistributionBase.h"

int Component::ParticleEmmiter::shader = 0;
Primative::Buffers::VertexBuffer Component::ParticleEmmiter::quadBuffer = {};

Component::ParticleEmmiter::ParticleEmmiter() : ComponetBase(), particleCount(0), particles(), looping(false), duration(0.0f), position(nullptr),
offset(0.0f), albedo(nullptr), spawnRate(1.0f), lifeTime(0.0f), timeBetweenSpawn(0.0f), numberAlive(0), playing(false), shape(nullptr), itteration(1)
{
	timeBetweenSpawn = 1.0f / lifeTime;

	if (NOT shader) {
		shader = ResourceLoader::getShader("ParticleShader");
		quadBuffer = ResourceLoader::getBuffer(ResourceLoader::getModel("plane.dae").getBuffers()[0]);
	}
}

void Component::ParticleEmmiter::sortParticles()
{
	Vector3 camPos = parent->getScene()->getMainCamera()->getPos();
	std::sort(particles.begin(), particles.end(), [&](Particles::Particle& a, Particles::Particle& b)
		{
			float dist1 = glm::length2(getParticlePosition(a) - camPos);
			float dist2 = glm::length2(getParticlePosition(b) - camPos);
			return dist1 > dist2;
		}
	);
}

Component::ParticleEmmiter::ParticleEmmiter(Int numberOfParticels, Float duration, bool looping, Float spawnRate) : ParticleEmmiter()
{
	this->particleCount = numberOfParticels;
	this->duration = duration;
	this->looping = looping;
	this->spawnRate = spawnRate;
	particles.reserve(numberOfParticels);
	for (int i = 0; i < numberOfParticels; i++) {
		particles.push_back(Particles::Particle());
	}
	playing = true;
}

glm::vec3 Component::ParticleEmmiter::getParticlePosition(const Particles::Particle& particle)
{
	return particle.getRelativePosition() + offset + *position;
}

glm::vec3 Component::ParticleEmmiter::getVelocityDistributed() const
{
	if (NOT shape) {
		return glm::vec3(0);
	}
	return shape->getNormalised() * 2.0f;
}

void Component::ParticleEmmiter::setParent(GameObject* parent) {
	ComponetBase::setParent(parent);
	position = &parent->getTransform()->Position;
}

void Component::ParticleEmmiter::update(float deltaTime)
{
	if (playing) {
		lifeTime += deltaTime;
		if (lifeTime >= duration * itteration) {
			itteration++;
		}
		bool canSpawn = lifeTime >= spawnRate * numberAlive AND (looping OR lifeTime <= duration);
		for (auto& part : particles) {
			part.update(deltaTime);
			if (NOT part.getIsAlive() AND canSpawn) {
				glm::vec3 vel = getVelocityDistributed();
				vel = glm::normalize(vel) * 2.0f;
				part.spawn(vel);
				numberAlive++;
				canSpawn = false;
			}
		}
	}

}
void Component::ParticleEmmiter::render(float deltaTime)
{
	sortParticles();
	Render::Shading::Manager::setActive(shader);
	int i = 0;
	for (auto& part : particles) {
		if (NOT part.getIsAlive())
			continue;
		Render::Shading::Manager::setValue("positions[" + std::to_string(i) + "]", getParticlePosition(part));
		i++;
	}
	unsigned unit = 0;
	if (albedo) {
		albedo->tryBindTexture(unit);
		unit = 0;
		Render::Shading::Manager::setValue("albedo", *albedo, unit);
	}
	quadBuffer.bind();
	glDrawElementsInstanced(quadBuffer.getDrawType(), quadBuffer.getIndicesCount(), GL_UNSIGNED_INT, 0, i);
	quadBuffer.unBind();
}
void Component::ParticleEmmiter::restart()
{
	for (auto& part : particles) {
		part.reset();
	}
}
void Component::ParticleEmmiter::cleanUp()
{
	for (auto& part : particles) {
		part.cleanUp();
	}
}

void Component::ParticleEmmiter::play()
{
	lifeTime = 0;
	playing = true;
}
void Component::ParticleEmmiter::pause()
{
	playing = false; 
}
void Component::ParticleEmmiter::togglePlay()
{
	playing = NOT playing;
}

Unsigned Component::ParticleEmmiter::getShader()
{
	return shader;
}

void Component::ParticleEmmiter::setShader(Unsigned shader)
{
	ParticleEmmiter::shader = shader;
}

void Component::ParticleEmmiter::setAlbedo(Materials::MatItemBase<glm::vec4>* albedo)
{
	this->albedo = albedo;
}

void Component::ParticleEmmiter::setShape(Particles::DistributionBase* shape)
{
	this->shape = shape;
}
