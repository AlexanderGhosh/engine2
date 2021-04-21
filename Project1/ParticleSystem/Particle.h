#pragma once
#include "../Utils/General.h"

namespace Particles {
	class Particle
	{
	private:
		glm::vec3 velocity;
		glm::vec3 relativePosition;
		float life, maxLife;
		bool isAlive;
	public:
		Particle();
		Particle(Vector3 relativePosition, Vector3 velocity, Float maxLife);
		~Particle() = default;
		void update(float deltaTime);
		void reset();
		void cleanUp();
		void spawn(Vector3 vel);
		
		// setters
		void setRelativePosition(Vector3 pos);
		void setVelocity(Vector3 vel);

		// getters
		Vector3 getRelativePosition() const;
		Vector3 getVelocity() const;
		Float getLife() const;
		const bool getIsAlive() const;

		// adders
		void addVelocity(Vector3 vel);
	};
};

