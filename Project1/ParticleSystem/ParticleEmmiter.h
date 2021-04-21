#pragma once
#include <map>
#include "../Componets/Componets.h"
#include "../Primatives/Buffers/VertexBuffer.h"
#include "Particle.h"

namespace Particles {
	class Particle;
	class DistributionBase;
}
namespace Materials {
	template<class T>
	class MatItemBase;
}
namespace Component {
#define MAX_PARTICLES 200

	class ParticleEmmiter : public ComponetBase {
	private:
		static int shader;
		static Primative::Buffers::VertexBuffer quadBuffer;
		std::map<float, Particles::Particle> particles;
		bool looping, playing;
		int particleCount, numberAlive;
		float duration, spawnRate, lifeTime, timeBetweenSpawn;
		glm::vec3* position;
		glm::vec3 offset;
		Materials::MatItemBase<glm::vec4>* albedo;
		Particles::DistributionBase* shape;
		glm::vec3 getParticlePosition(const Particles::Particle& particle);
		glm::vec3 getVelocityDistributed() const;
		void sortParticles();
	public:
		ParticleEmmiter();
		ParticleEmmiter(Int numberOfParticels, Float duration, bool looping, Float spawnRate = 1);
		~ParticleEmmiter() = default;

		void setParent(GameObject* parent);
		
		inline Type getType() const { return Type::ParticleEmmiter; };
		void update(float deltaTime);
		void render(float deltaTime);
		void restart();
		void cleanUp();

		// setters
		void setAlbedo(Materials::MatItemBase<glm::vec4>* albedo);
		void setShape(Particles::DistributionBase* shape);
	};
}

