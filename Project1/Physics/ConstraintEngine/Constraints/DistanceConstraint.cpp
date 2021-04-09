#include "DistanceConstraint.h"
#include "../../../Componets/Rigidbody.h"
#include "../../Engine.h"
#include <gtx/matrix_cross_product.hpp>

Physics::Constraints::DistanceConstraint::DistanceConstraint(Vector3 relPosA, Vector3 relPosB, float targetLength) : Constraint(), relPosA(relPosA), relPosB(relPosB), targetLength(targetLength)
, r1(), r2(), globalPosA(), globalPosB(), ab()
{
	type = Type::Seperation;
}

Physics::Constraints::DistanceConstraint::DistanceConstraint(Component::RigidBody* a, Component::RigidBody* b, Vector3 relPosA, Vector3 relPosB, float targetLength) : DistanceConstraint(relPosA, relPosB, targetLength)
{
	body1 = a;
	body2 = b;
}

void Physics::Constraints::DistanceConstraint::solve(const float& dt)
{
	const BigMaths::vec12 V = getVelocityVector();
	const BigMaths::mat12 inv_M = getInvMassMatrix();
	buildJacobian();
	if (BigMaths::dot(Jacobian, V) == 0) {
		return;
	}
	float lambda = getLagrangian(V, inv_M, {});
	BigMaths::vec12 deltaV = (inv_M * Jacobian) * lambda;

	float C = BigMaths::dot(Jacobian, V + deltaV);
	Utils::Log("Velocity: " + std::to_string(C));
	//assert(Utils::round(t, 4) == 0 AND "Constraint not corrected");
	body1->velocityAdder(deltaV.get(2));
	body2->velocityAdder(deltaV.get(0));

	const glm::vec3 x1 = body1->getCOM() + relPosA * body1->getRotationMatrix();
	const glm::vec3 x2 = body2->getCOM() + relPosB * body2->getRotationMatrix();
	C = powf(glm::dot(x1 - x2, x1 - x2), 0.5f);
	if (x2 == x1) C = 0;
	Utils::Log("Positional: " + std::to_string(C));
	Utils::Log("------------------------------------------------------------");
}


void Physics::Constraints::DistanceConstraint::cleanUp()
{
	body1 = nullptr;
	body2 = nullptr;
}

void Physics::Constraints::DistanceConstraint::buildJacobian()
{
	const glm::vec3 x1 = body1->getCOM() + relPosA * body1->getRotationMatrix();
	const glm::vec3 x2 = body2->getCOM() + relPosB * body2->getRotationMatrix();
	const glm::vec3 d = Utils::normalize(x2 - x1);
	Jacobian.set(0, -d);
	Jacobian.set(1, -glm::cross(relPosA * body1->getRotationMatrix(), d));
	Jacobian.set(2, d);
	Jacobian.set(3, glm::cross(relPosB * body2->getRotationMatrix(), d));
}

const float Physics::Constraints::DistanceConstraint::getBias(const Physics::CollisionManfold& d) const
{
	const glm::vec3 x1 = body1->getCOM() + relPosA * body1->getRotationMatrix();
	const glm::vec3 x2 = body2->getCOM() + relPosB * body2->getRotationMatrix();
	float offset = glm::length(x2 - x1) - targetLength;
	return -(BaumgarteScalar / Physics::Engine::getDeltaTime()) * offset;
}

const float Physics::Constraints::DistanceConstraint::getLagrangian(Vector12 V, Matrix12 M, const CollisionManfold& manafoild) const
{
	const BigMaths::vec12 t = Jacobian * M;
	const float effectiveMass = BigMaths::dot(Jacobian, M * Jacobian);
	const float numerator = -(BigMaths::dot(Jacobian, V) + getBias(manafoild));
	return numerator / effectiveMass;
}
