#include "AudioReciever.h"
#include <al.h>
#include "../GameObject/GameObject.h"
#include "RigidBody.h"
#include "Camera.h"

Component::AudioReciever::AudioReciever() : ComponetBase(), placeHolder(0)
{

}

void Component::AudioReciever::update(float deltaTime)
{
	Component::Transform* transofrm = parent->getLocalTransform();
	Component::Camera* camera = parent->getComponet<Component::Camera>();
	Component::RigidBody* rb = parent->getComponet<Component::RigidBody>();
	alListener3f(AL_POSITION, transofrm->Position.x, transofrm->Position.y, transofrm->Position.z);
	if (rb) {
		alListener3f(AL_VELOCITY, rb->getVelocity().x, rb->getVelocity().y, rb->getVelocity().z);
	}
	std::array<glm::vec3, 2> orientation = {
		glm::vec3(0), Utils::yAxis()
	};
	if (camera) {
		orientation[0] = camera->getForward();
	}
	alListenerfv(AL_ORIENTATION, &orientation[0].x);
}

void Component::AudioReciever::cleanUp()
{
}
