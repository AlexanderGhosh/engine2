#pragma once
#include "../Componets/Componets.h"
#include "../Utils/General.h"
#include "../Primatives/Buffers/VertexBuffer.h"

namespace Component {
#define MAX_PARTICLES 200
	class ParticleEmmiter : public ComponetBase
	{
	private:
		std::vector<Component::Transform> particleTransforms;
		std::vector<float> life;
		int maxParticelCount, maxLife;
		glm::vec3 velocity, orgiCenter, oridRadii;
		bool doLoop;
		static int shader;
		static Primative::Buffers::VertexBuffer quadBuffer;

		void spawn();
	public:
		ParticleEmmiter();
		ParticleEmmiter(Unsigned numberOfParticles, Unsigned life, Vector3 center, Vector3 radii);
		~ParticleEmmiter() = default;
		void update(float deltaTime);
		void drawParticles();
		inline Type getType() const { return Type::ParticleEmmiter; };
		void cleanUp();
		void restart();
		void loop();
		bool isDead() const;
	};
}

