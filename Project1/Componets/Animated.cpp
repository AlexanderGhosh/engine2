#include "Animated.h"
#include "../Rendering/Animation/Animation.h"
#include "../Rendering/Shading/Manager.h"
#include "../Primatives/Buffers/ArrayBuffer.h"

Primative::Buffers::TextureBuffer Component::Animated::bonesBuffer = {};
bool Component::Animated::initalized = false;

bool Component::Animated::isActive()
{
	return running AND getActiveAnimation();
}

Component::Animated::Animated() : animations(), activeAnimation(""), runTime(0), running(false)
{
	if (NOT initalized) {
		bonesBuffer = Primative::Buffers::TextureBuffer("f", sizeof(float) * MAX_NUM_BONES * 16);
	}
}

void Component::Animated::startAnimation(String name, bool start)
{
	if (NOT animations.size())
		return;
	activeAnimation = name;
	running = start;
}

void Component::Animated::stopAnimation()
{
	running = false;
}

void Component::Animated::playAnimation()
{
	running = false;
}

void Component::Animated::togglePlay()
{
	running = NOT running;
}

const Render::Animation::KeyFrame& Component::Animated::nextFrame()
{
	/*runTime++;
	runTime %= animations[activeAnimation].getFrameCount();*/
	return getCurrentFrame();
}

const Render::Animation::KeyFrame Component::Animated::getCurrentFrame()
{
	if (NOT isActive())
		return {};
	return getActiveAnimation()->getFrame(runTime);
}

void Component::Animated::addAnimation(Render::Animation::Animation* animation)
{
	if(animation)
		animations.insert(animations.end(), { animation->getName(), animation });
}

void Component::Animated::cleanUp()
{
	animations.clear();
}

void Component::Animated::update(float deltaTime)
{
	if (running AND getActiveAnimation()) {
		runTime += deltaTime;
		runTime = fmod(runTime, getActiveAnimation()->getDuration());
	}
}

const Render::Animation::Animation* Component::Animated::getActiveAnimation()
{
	const int s = animations.size();
	auto res = animations[activeAnimation];
	if (s != animations.size()) {
		animations.erase(activeAnimation);
		return nullptr;
	}
	return res;
}

void Component::Animated::bind()
{
	if (NOT isActive())
		return;
	// Render::Shading::Manager::setValue("bones", this->getCurrentFrame());
	auto t = this->getCurrentFrame();
	// Utils::reverse(t.translations.begin(), t.translations.end());
	bonesBuffer.fill(&t.translations[0], sizeof(float) * 16 * t.translations.size());
	bonesBuffer.bind();
	Render::Shading::Manager::setValue("bones", 0);
}

void Component::Animated::unbind()
{
	bonesBuffer.unBind();
}
