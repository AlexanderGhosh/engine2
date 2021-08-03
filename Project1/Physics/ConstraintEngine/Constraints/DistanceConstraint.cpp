#include "DistanceConstraint.h"
#include "../../../Componets/Rigidbody.h"

Physics::Constraints::DistanceConstraint::DistanceConstraint(Vector3 relPosA, Vector3 relPosB, float targetLength) : Constraint(), relPosA(relPosA), relPosB(relPosB), targetLength(targetLength)
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
	glm::vec3 r1 = body1->getRotation() * relPosA;
	glm::vec3 r2 = body2->getRotation() * relPosB;

	glm::vec3 globalOnA = r1 + body1->getCOS();
	glm::vec3 globalOnB = r2 + body2->getCOS();

	const glm::vec3 ab = globalOnB - globalOnA;
	const glm::vec3 abn = glm::normalize(ab);

	/*glm::vec3 v0 = body1->getVelocity() + glm::cross(body1->getAngularVelocity(), r1);
	glm::vec3 v1 = body2->getVelocity() + glm::cross(body2->getAngularVelocity(), r2);

	float abnVel = glm::dot(v0 - v1, abn);

	float invConstraintMassLin = body1->getInvMass() + body2->getInvMass();

	float invConstraintMassRot = 
		glm::dot(abn, glm::cross(body1->getInvInertia_G() * glm::cross(r1, abn), r1)) +
		glm::dot(abn, glm::cross(body2->getInvInertia_G() * glm::cross(r2, abn), r2));

	float constraintMass = invConstraintMassLin + invConstraintMassRot;

	if (constraintMass > 0) {
		float b = 0.0f;
		// -Optional -
		float distance_offset = glm::length(ab) - targetLength;
		float baumgarte_scalar = 0.1f;
		b = -(baumgarte_scalar / dt) * distance_offset;
		
		// -Eof Optional -
		
		// Compute velocity impulse (jn)
		// In order to satisfy the distance constraint we need
		// to apply forces to ensure the relative velocity
		// ( abnVel ) in the direction of the constraint is zero .
		// So we take inverse of the current rel velocity and
		// multiply it by how hard it will be to move the objects .
		
		// Note : We also add in any extra energy to the system
		// here , e.g. baumgarte (and later elasticity )
		
		float jn = -(abnVel + b) / constraintMass;
		
		// Apply linear velocity impulse
		
		// std::cout << glm::to_string(abn * (body1->getInvMass() * jn)) << std::endl;

		body1->velocityAdder(abn * (body1->getInvMass() * jn));
		body2->velocityAdder(-abn * (body2->getInvMass() * jn));
		
		// Apply rotational velocity impulse
		
		body1->angularVelAdder((body1->getInvInertia_G() * glm::cross(r1, abn * jn)));
		body2->angularVelAdder(-(body2->getInvInertia_G() * glm::cross(r2, abn * jn)));
	}
	*/
	float b = -glm::dot(abn, body1->getVelocity() + glm::cross(body1->getAngularVelocity(), relPosA) - 
		body2->getVelocity() - glm::cross(body2->getAngularVelocity(), relPosB));

	float a = 0;
	const std::array<glm::mat3, 4> inv_massMatrix = {
		glm::mat3(body1->getInvMass()),
		glm::mat3(body1->getInvInertia_G()),
		glm::mat3(body2->getInvMass()),
		glm::mat3(body2->getInvInertia_G()),
	};
	const std::array<glm::vec3, 4> jacobian = {
		abn, glm::cross(relPosA, ab), -abn, -glm::cross(relPosB, ab)
	};
	for (int i = 0; i < 4; i++) {
		a += glm::dot(jacobian[i] * inv_massMatrix[i], jacobian[i]);
	}

	/*float a = body1->getInvMass() + body2->getInvMass();
	a += body1->getInvInertia_G() * glm::pow(glm::cross(abn, body1->getAngularVelocity()), glm::vec3(2.0f));
	a -= body2->getInvInertia_G() * glm::pow(glm::cross(abn, body2->getAngularVelocity()), glm::vec3(2.0f));*/

	float bias = 0.0f;
	// -Optional -
	float distance_offset = glm::length(ab) - targetLength;
	float baumgarte_scalar = 0.1f;
	bias = -(baumgarte_scalar / dt) * distance_offset;

	float lambda = b / a;
	lambda += bias;

	body1->velocityAdder(abn  * (body1->getInvMass() * lambda));
	body2->velocityAdder(-abn * (body2->getInvMass() * lambda));
}

void Physics::Constraints::DistanceConstraint::cleanUp()
{
}

void Physics::Constraints::DistanceConstraint::buildJacobian()
{
}
